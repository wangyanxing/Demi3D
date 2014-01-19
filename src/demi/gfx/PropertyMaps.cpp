
#include "PropertyMaps.h"
#include "AssetManager.h"
#include "Image.h"

namespace Demi 
{
    DiByteMap::~DiByteMap()
    {
        DI_ASSERT(mPixels);
        if (mReleaseData)
        {
            DI_DELETE[] static_cast<uint8*>(mPixels->data);
        }
        DI_DELETE mPixels;
    }

    DiByteMap::DiByteMap(DiTexturePtr map, MapChannel channel)
    {
        DI_ASSERT(map);
        mFilter = MAPFILTER_BILINEAR;

        mReleaseData = true;

        mPixels = DI_NEW DiPixelBox(DiBox(0, 0, map->GetWidth(), map->GetHeight()), PF_L8);
        mPixels->data = DI_NEW uint8[mPixels->GetConsecutiveSize()];

        if (channel == CHANNEL_COLOR)
        {
            map->CopyToMemory(*mPixels);
        }
        else 
        {
            DiPixelBox tmpPixels(DiBox(0, 0, map->GetWidth(), map->GetHeight()), PF_A8B8G8R8);
            tmpPixels.data = new uint8[tmpPixels.GetConsecutiveSize()];
            map->CopyToMemory(tmpPixels);

            size_t channelOffset;
            switch (channel)
            {
            case CHANNEL_ALPHA: channelOffset = 3; break;
            case CHANNEL_RED:    channelOffset = 2; break;
            case CHANNEL_GREEN: channelOffset = 1; break;
            case CHANNEL_BLUE:    channelOffset = 0; break;
            default:
                DI_WARNING("Undefined channel!");
                break;
            }

            uint8 *inputPtr        = (uint8*)tmpPixels.data + channelOffset;
            uint8 *outputPtr    = (uint8*)mPixels->data;
            uint8 *outputEndPtr = outputPtr + mPixels->GetConsecutiveSize();
            while (outputPtr    != outputEndPtr)
            {
                *outputPtr++ = *inputPtr;
                inputPtr += 4;
            }

            DI_DELETE[] static_cast<uint8*>(tmpPixels.data);
        }
    }

    DiByteMap::DiByteMap( BYTE* rawData, uint32 width, uint32 height )
    {
        mReleaseData = false;

        mFilter = MAPFILTER_BILINEAR;

        mPixels = DI_NEW DiPixelBox(DiBox(0, 0, width, height), PF_L8);
        mPixels->data = rawData;
    }

    float DiByteMap::GetDensityAt_Unfiltered(float x, float z, const DiFloatRect &mapBounds)
    {
        DI_ASSERT(mPixels);

        if(x < mapBounds.left || x >= mapBounds.right || z < mapBounds.top || z >= mapBounds.bottom)
        {
            return 0.f;
        }

        size_t mapWidth = mPixels->GetWidth(), mapHeight = mPixels->GetHeight();

        size_t xindex = size_t(mapWidth * (x - mapBounds.left) / mapBounds.Width());
        size_t zindex = size_t(mapHeight * (z - mapBounds.top) / mapBounds.Height());

        uint8 *data = reinterpret_cast<uint8*>(mPixels->data);
        return data[mapWidth * zindex + xindex] * 0.00392157f; // 1/255.0f;
    }

    float DiByteMap::GetDensityAt_Bilinear(float x, float z, const DiFloatRect &mapBounds)
    {
        DI_ASSERT(mPixels);

        if(x < mapBounds.left || x >= mapBounds.right || z < mapBounds.top || z >= mapBounds.bottom)
        {
            return 0.f;
        }

        uint32 mapWidth = (uint32)mPixels->GetWidth();
        uint32 mapHeight = (uint32)mPixels->GetHeight();


        float xIndexFloat = (mapWidth * (x - mapBounds.left) / mapBounds.Width()) - 0.5f;
        float zIndexFloat = (mapHeight * (z - mapBounds.top) / mapBounds.Height()) - 0.5f;

        uint32 xIndex = (uint32)xIndexFloat;
        uint32 zIndex = (uint32)zIndexFloat;
        if (xIndex < 0 || zIndex < 0 || xIndex >= mapWidth-1 || zIndex >= mapHeight-1)
            return 0.0f;

        float xRatio = xIndexFloat - xIndex;
        float xRatioInv = 1.f - xRatio;
        float zRatio = zIndexFloat - zIndex;
        float zRatioInv = 1.f - zRatio;

        uint8 *data = (uint8*)mPixels->data;

        float val11 = data[mapWidth * zIndex + xIndex]      * 0.0039215686274509803921568627451f;
        float val21 = data[mapWidth * zIndex + xIndex + 1]  * 0.0039215686274509803921568627451f;
        float val12 = data[mapWidth * ++zIndex + xIndex]    * 0.0039215686274509803921568627451f;
        float val22 = data[mapWidth * zIndex + xIndex + 1]  * 0.0039215686274509803921568627451f;

        float val1 = xRatioInv * val11 + xRatio * val21;
        float val2 = xRatioInv * val12 + xRatio * val22;

        return zRatioInv * val1 + zRatio * val2;
    }

    float DiByteMap::GetDensityAt( float x, float z, const DiFloatRect &mapBounds )
    {
        if (mFilter == MAPFILTER_NONE)
        {
            return GetDensityAt_Unfiltered(x, z, mapBounds);
        }
        else
        {
            return GetDensityAt_Bilinear(x, z, mapBounds);
        }
    }

    DiPixelBox* DiByteMap::GetPixelBox()
    {
        return mPixels;
    }

    DiMap<DiString, DiColorMap*> DiColorMap::sSelfList;

    DiColorMap *DiColorMap::Load(const DiString &fileName, MapChannel channel)
    {
        DiTexturePtr map = DiAssetManager::GetInstance().GetAsset<DiTexture>(fileName);

        return Load(map, channel);
    }

    DiColorMap *DiColorMap::Load(DiTexturePtr texture, MapChannel channel)
    {
        DiString key ;
        key.Format("%s%d",texture->GetName().c_str(),(int)channel);

        DiMap<DiString, DiColorMap*>::iterator i;
        i = sSelfList.find(key);

        DiColorMap *m;
        if (i != sSelfList.end())
            m = i->second;
        else
            m = new DiColorMap(texture, channel);

        ++(m->mRefCount);
        return m;
    }

    void DiColorMap::Unload()
    {
        --mRefCount;
        if (mRefCount == 0)
            delete this;
    }

    DiColorMap::~DiColorMap()
    {
        DI_ASSERT(mPixels);
        delete[] static_cast<uint8*>(mPixels->data);
        delete mPixels;

        sSelfList.erase(mSelfKey);
    }

    DiColorMap::DiColorMap(DiTexturePtr map, MapChannel channel)
    {
        DI_ASSERT(map);
        mFilter = MAPFILTER_BILINEAR;

        mSelfKey.Format("%s%d",map->GetName().c_str(),(int)channel);
        sSelfList.insert(std::pair<DiString, DiColorMap*>(mSelfKey, this));
        mRefCount = 0;

        mPixels = new DiPixelBox(DiBox(0, 0, map->GetWidth(), map->GetHeight()), PF_A8R8G8B8);
        mPixels->data = new uint8[mPixels->GetConsecutiveSize()];

        if (channel == CHANNEL_COLOR)
        {
            map->CopyToMemory(*mPixels);
        }
        else 
        {
            DiPixelBox tmpPixels(DiBox(0, 0, map->GetWidth(), map->GetHeight()), PF_A8R8G8B8);
            tmpPixels.data = new uint8[tmpPixels.GetConsecutiveSize()];
            map->CopyToMemory(tmpPixels);

            size_t channelOffset;
            switch (channel)
            {
            case CHANNEL_ALPHA: channelOffset = 3; break;
            case CHANNEL_RED:    channelOffset = 2; break;
            case CHANNEL_GREEN: channelOffset = 1; break;
            case CHANNEL_BLUE:    channelOffset = 0; break;
            default:
                DI_WARNING("Undefined channel!");
                break;
            }

            uint8 *inputPtr = (uint8*)tmpPixels.data + channelOffset;
            uint8 *outputPtr = (uint8*)mPixels->data;
            uint8 *outputEndPtr = outputPtr + mPixels->GetConsecutiveSize();
            while (outputPtr != outputEndPtr)
            {
                *outputPtr++ = *inputPtr;
                *outputPtr++ = *inputPtr;
                *outputPtr++ = *inputPtr;
                *outputPtr++ = 0xFF;
                inputPtr += 4;
            }

            DI_DELETE[] static_cast<uint8*>(tmpPixels.data);
        }
    }

    uint32 DiColorMap::GetColorAt_NoFilt(float x, float z, const TRect<float> &mapBounds)
    {
        DI_ASSERT(mPixels);

        if(x < mapBounds.left || x >= mapBounds.right || z < mapBounds.top || z >= mapBounds.bottom)
        {
            return 0xFFFFFFFF;
        }

        uint32 mapWidth = (uint32)mPixels->GetWidth();
        uint32 mapHeight = (uint32)mPixels->GetHeight();

        uint32 xindex = uint32(mapWidth * (x - mapBounds.left) / mapBounds.Width());
        uint32 zindex = uint32(mapHeight * (z - mapBounds.top) / mapBounds.Height());

        uint32 *data = (uint32*)mPixels->data;
        return data[mapWidth * zindex + xindex];
    }

    uint32 DiColorMap::InterpolateColor(uint32 color1, uint32 color2, float ratio, float ratioInv)
    {
        uint8 a1 = (uint8)(color1 & 0xFF);
        uint8 b1 = (uint8)(color1 >> 8 & 0xFF);
        uint8 c1 = (uint8)(color1 >> 16 & 0xFF);
        uint8 d1 = (uint8)(color1 >> 24 & 0xFF);

        uint8 a2 = (uint8)(color2 & 0xFF);
        uint8 b2 = (uint8)(color2 >> 8 & 0xFF);
        uint8 c2 = (uint8)(color2 >> 16 & 0xFF);
        uint8 d2 = (uint8)(color2 >> 24 & 0xFF);

        uint8 a = uint8(ratioInv * a1 + ratio * a2);
        uint8 b = uint8(ratioInv * b1 + ratio * b2);
        uint8 c = uint8(ratioInv * c1 + ratio * c2);
        uint8 d = uint8(ratioInv * d1 + ratio * d2);

        return a | (b << 8) | (c << 16) | (d << 24);
    }

    uint32 DiColorMap::GetColorAt_Bilinear(float x, float z, const TRect<float> &mapBounds)
    {
        DI_ASSERT(mPixels);

        if(x < mapBounds.left || x >= mapBounds.right || z < mapBounds.top || z >= mapBounds.bottom)
        {
            return 0xFFFFFFFF;
        }

        uint32 mapWidth = (uint32)mPixels->GetWidth();
        uint32 mapHeight = (uint32)mPixels->GetHeight();
        float xIndexFloat = (mapWidth * (x - mapBounds.left) / mapBounds.Width()) - 0.5f;
        float zIndexFloat = (mapHeight * (z - mapBounds.top) / mapBounds.Height()) - 0.5f;

        uint32 xIndex = (uint32)xIndexFloat;
        uint32 zIndex = (uint32)zIndexFloat;
        if (xIndex < 0 || zIndex < 0 || xIndex > mapWidth-1 || zIndex > mapHeight-1)
            return 0xFFFFFFFF;

        float xRatio = xIndexFloat - xIndex;
        float xRatioInv = 1.f - xRatio;
        float zRatio = zIndexFloat - zIndex;
        float zRatioInv = 1.f - zRatio;

        uint32 *data = (uint32*)mPixels->data;

        uint32 val11 = data[mapWidth * zIndex + xIndex];
        uint32 val21 = data[mapWidth * zIndex + xIndex + 1];
        uint32 val12 = data[mapWidth * ++zIndex + xIndex];
        uint32 val22 = data[mapWidth * zIndex + xIndex + 1];

        uint32 val1 = InterpolateColor(val11, val21, xRatio, xRatioInv);
        uint32 val2 = InterpolateColor(val12, val22, xRatio, xRatioInv);

        uint32 val = InterpolateColor(val1, val2, zRatio, zRatioInv);

        return val;
    }
}
