/**********************************************************************
This source file is a part of Demi3D
   __  ___  __  __  __
  |  \|_ |\/||   _)|  \ 
  |__/|__|  ||  __)|__/ 

Copyright (c) 2013-2014 Demi team
https://github.com/wangyanxing/Demi3D

Released under the MIT License
https://github.com/wangyanxing/Demi3D/blob/master/License.txt
***********************************************************************/

#ifndef DiPropertyMaps_h__
#define DiPropertyMaps_h__


#include "Texture.h"

namespace Demi
{
    enum MapChannel
    {
        CHANNEL_RED,
        CHANNEL_GREEN,
        CHANNEL_BLUE,
        CHANNEL_ALPHA,
        CHANNEL_COLOR
    };

    enum MapFilter
    {
        MAPFILTER_NONE,
        MAPFILTER_BILINEAR
    };

    class DiByteMap
    {
    public:

        DiByteMap(DiTexturePtr texture, MapChannel channel);

        DiByteMap(BYTE* rawData, uint32 width, uint32 height);

        ~DiByteMap();

        void                SetFilter(MapFilter filter) { mFilter = filter; }

        MapFilter           GetFilter() { return mFilter; }

        DiPixelBox*         GetPixelBox();

        float               GetDensityAt(float x, float z, const DiFloatRect &mapBounds);

        float               GetDensityAt_Unfiltered(float x, float z, const DiFloatRect &mapBounds);

        float               GetDensityAt_Bilinear(float x, float z, const DiFloatRect &mapBounds);

    private:
        
        MapFilter           mFilter;

        DiPixelBox*         mPixels;

        bool                mReleaseData;
    };

    class DiColorMap
    {
    public:
        static DiColorMap*  Load(const DiString &fileName, MapChannel channel = CHANNEL_COLOR);

        static DiColorMap*  Load(DiTexturePtr texture, MapChannel channel = CHANNEL_COLOR);

        void                Unload();

        void                SetFilter(MapFilter filter) { this->mFilter = filter; }

        MapFilter           GetFilter() { return mFilter; }

        DiPixelBox*         GetPixelBox()
        {
            return mPixels;
        }

        uint32              GetColorAt(float x, float z, const TRect<float> &mapBounds)
        {
            return mFilter == MAPFILTER_NONE ? GetColorAt_NoFilt(x, z, mapBounds) : GetColorAt_Bilinear(x, z, mapBounds);
        }

        DiColor             GetColorAt_Unpacked(float x, float z, const TRect<float> &mapBounds)
        {
            uint32 c = mFilter == MAPFILTER_NONE ? GetColorAt(x, z, mapBounds) : GetColorAt_Bilinear(x, z, mapBounds);
            float r, g, b, a;
            b = ((c) & 0xFF) * 0.0039215686f;
            g = ((c >> 8) & 0xFF) * 0.0039215686f;
            r = ((c >> 16) & 0xFF) * 0.0039215686f;
            a = ((c >> 24) & 0xFF) * 0.0039215686f;

            return DiColor(r, g, b, a);
        }

    private:
        DiColorMap(DiTexturePtr map, MapChannel channel);

        ~DiColorMap();

        uint32              InterpolateColor(uint32 color1, uint32 color2, float ratio, float ratioInv);

        uint32              GetColorAt_NoFilt(float x, float z, const TRect<float> &mapBounds);

        uint32              GetColorAt_Bilinear(float x, float z, const TRect<float> &mapBounds);

        static DiMap<DiString, DiColorMap*> sSelfList;

        DiString            mSelfKey;

        uint32              mRefCount;

        MapFilter           mFilter;

        DiPixelBox*         mPixels;

        TRect<float>        mMapBounds;
    };

}

#endif
