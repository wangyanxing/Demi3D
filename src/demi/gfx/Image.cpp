
#include "GfxPch.h"
#include "Image.h"
#include "Texture.h"
#include "AssetManager.h"

#include "nv_dds.h"
#include "targa.h"

namespace Demi 
{
    static uint32 ComputeCompressedDimension(uint32 dimension)
    {
        if(dimension < 4)
            dimension = 4;
        else
        {
            uint32 mod = dimension % 4;
            if(mod) dimension += 4 - mod;
        }
        return dimension;
    }

    //////////////////////////////////////////////////////////////////////////

    DiImage::DiImage( const DiString& name ):
        mHeight(0),
        mWidth(0)
    {
        mImageData = DiAssetManager::GetInstance().OpenArchive(name);
    }

    DiImage::DiImage( DiDataStreamPtr stream ):
        mHeight(0),
        mWidth(0),
        mImageData(stream)
    {
    }

    DiImage::~DiImage()
    {

    }

    BOOL DiImage::LoadToTexture( DiTexture* texture )
    {
        if (!mImageData)
        {
            return FALSE;
        }

        DiString name = mImageData->GetName();
        name.ToLower();
        
        if (name.CheckFileExtension("tga"))
        {
            ParseTga(texture);
        }
        else if (name.CheckFileExtension("dds"))
        {
            ParseDDS(texture);
        }
    
        return FALSE;
    }

    void DiImage::ParseTga(DiTexture* texture)
    {
        tga_image* image = new tga_image();
        bool ok = (TGA_NOERR == tga_read_from_stream( image, mImageData.get() ));

        tga_flip_vert( image );

        DI_ASSERT(ok);
        if( ok )
        {
            int componentCount = image->pixel_depth/8;
            if( componentCount == 3 || componentCount == 4 )
            {
                mWidth = image->width;
                mHeight = image->height;
                if(texture)
                {
                    texture->Release();
                    texture->SetDimensions(image->width, image->height);
                    texture->SetFormat(PF_A8R8G8B8);
                    texture->SetNumLevels(1);
                    texture->SetResourceUsage(RU_WRITE_ONLY);
                    texture->CreateTexture();

                    void *buffer = texture->LockLevel(0);
                    DI_ASSERT(buffer);
                    if(buffer)
                    {
                        uint8       *levelDst    = (uint8*)buffer;
                        const uint8 *levelSrc    = (uint8*)image->image_data;
                        const uint32 levelWidth  = texture->GetWidthInBlocks();
                        const uint32 levelHeight = texture->GetHeightInBlocks();
                        const uint32 rowSrcSize  = levelWidth * texture->GetBlockSize();
                        for(uint32 row=0; row<levelHeight; row++)
                        { 
                            if( componentCount == 3 )
                            {
                                for(uint32 col=0; col<levelWidth; col++)
                                {
                                    *levelDst++ = levelSrc[0];
                                    *levelDst++ = levelSrc[1];
                                    *levelDst++ = levelSrc[2];
                                    *levelDst++ = 0xFF; //alpha
                                    levelSrc += componentCount;
                                }
                            }
                            else
                            {
                                memcpy(levelDst, levelSrc, rowSrcSize);
                                levelDst += rowSrcSize;
                                levelSrc += rowSrcSize;
                            }
                        }
                    }
                    texture->UnlockLevel(0);
                }
            }
        }
        delete image;
    }

    void DiImage::ParseDDS(DiTexture* texture)
    {
        nv_dds::CDDSImage ddsimage;
        bool ok = ddsimage.load(mImageData.get(), false);
        DI_ASSERT(ok);

        if(ok)
        {
            nv_dds::TextureFormat ddsformat = ddsimage.get_format();
            DiPixelFormat format = PIXEL_FORMAT_MAX;
            switch(ddsformat)
            {
            case nv_dds::TextureBGR:          format = PF_R8G8B8;      break;
            case nv_dds::TextureBGRA:         format = PF_A8R8G8B8;    break;
            case nv_dds::TextureDXT1:         format = PF_DXT1;        break;
            case nv_dds::TextureDXT3:         format = PF_DXT3;        break;
            case nv_dds::TextureDXT5:         format = PF_DXT5;        break;
            case nv_dds::TextureLuminance:    format = PF_L8;          break;
            default:
                DI_ERROR("Unsupported texture pixel format");
            }

            if(texture)
            {
                texture->Release();
                texture->SetTextureType(ddsimage.is_cubemap() ? TEXTURE_CUBE : TEXTURE_2D);
                texture->SetDimensions(ddsimage.get_width(),ddsimage.get_height());
                texture->SetFormat(format);
                texture->SetResourceUsage(RU_WRITE_ONLY);
                texture->SetNumLevels(ddsimage.get_num_mipmaps()+1);
                texture->CreateTexture();
                DiTextureDrv* texDrv = texture->GetTextureDriver();

                int faces = ddsimage.is_cubemap()?6:1;

                for (int f = 0; f < faces; f++)
                {
                    void* levelSrc = nullptr;
                    if (ddsimage.is_cubemap())
                        levelSrc = (void*)ddsimage.get_cubemap_face(f);
                    else
                        levelSrc = (void*)ddsimage;

                    const uint32 levelWidth = texture->GetWidth();
                    const uint32 levelHeight = texture->GetHeight();

                    DiPixelBox pixbox(levelWidth, levelHeight, format, levelSrc);
                    texDrv->CopyFromMemory(pixbox, 0, f);

                    for (uint32 i = 1; i < texture->GetNumLevels(); i++)
                    {
                        const nv_dds::CSurface &surface = ddsimage.get_mipmap(i - 1, f);
                        void* curLevelSrc = (void*)surface;
                        const uint32 curLevelWidth = surface.get_width();
                        const uint32 curLevelHeight = surface.get_height();
                        DiPixelBox pixbox(curLevelWidth, curLevelHeight, format, curLevelSrc);
                        texDrv->CopyFromMemory(pixbox, i, f);
                    }
                }
            }
        }
    }

    size_t DiPixelBox::GetConsecutiveSize() const
    {
        return DiPixelBox::ComputeImageByteSize(GetWidth(), GetHeight(), format);
    }

    Demi::DiPixelBox DiPixelBox::GetSubVolume( const DiBox &def ) const
    {
        if(DiPixelBox::IsCompressedFormat(format))
        {
            if(def.left == left && def.top == top &&
                def.right == right && def.bottom == bottom)
            {
                return *this;
            }
            DI_ERROR("DiPixelBox::SubVolume() error");
        }
        if(!Contains(def))
        {
            DI_ERROR("DiPixelBox::SubVolume() error");
        }

        const size_t elemSize = GetNumElemBytes(format);
        DiPixelBox rval(def.GetWidth(), def.GetHeight(), format, 
            ((uint8*)data) + ((def.left-left)*elemSize)
            + ((def.top-top)*rowPitch*elemSize)
            );

        rval.rowPitch = rowPitch;
        rval.slicePitch = slicePitch;
        rval.format = format;

        return rval;
    }

    DiColor DiPixelBox::GetColourAt( size_t x, size_t y, size_t z )
    {
        DiColor cv;
        unsigned char pixelSize = (uint8)GetNumElemBytes(format);
        size_t pixelOffset = pixelSize * (z * slicePitch + y * rowPitch + x);
        UnpackColour(&cv, format, (unsigned char *)data + pixelOffset);
        return cv;
    }

    void DiPixelBox::SetColourAt( DiColor const &cv, size_t x, size_t y, size_t z )
    {
        unsigned char pixelSize = (uint8)GetNumElemBytes(format);
        size_t pixelOffset = pixelSize * (z * slicePitch + y * rowPitch + x);
        PackColour(cv, format, (unsigned char *)data + pixelOffset);
    }

    uint32 DiPixelBox::ComputeImageByteSize( uint32 width, uint32 height, DiPixelFormat format )
    {
        uint32 size = 0;
        uint32 numPixels = width * height ;
        switch(format)
        {
        case PF_DXT1:
            width   = ComputeCompressedDimension(width);
            height  = ComputeCompressedDimension(height);
            size    = ComputeImageByteSize(width, height, PF_A8R8G8B8) / 8;
            break;
        case PF_DXT3:
        case PF_DXT5:
            width   = ComputeCompressedDimension(width);
            height  = ComputeCompressedDimension(height);
            size    = ComputeImageByteSize(width, height, PF_A8R8G8B8) / 4;
            break;
        default:
            size    = GetNumElemBytes(format) * numPixels;
        }
        DI_ASSERT(size);
        return size;
    }

    uint32 DiPixelBox::GetLevelDimension( uint32 dimension, uint32 level )
    {
        dimension >>=level;

        if(!dimension) 
            dimension=1;

        return dimension;
    }

    BOOL DiPixelBox::IsCompressedFormat( DiPixelFormat fmt )
    {
        if (fmt == PF_DXT1 ||
            fmt == PF_DXT2 ||
            fmt == PF_DXT3 ||
            fmt == PF_DXT4 ||
            fmt == PF_DXT5
            )
        {
            return TRUE;
        }
        return FALSE;
    }

    uint32 DiPixelBox::GetFormatNumBlocks( uint32 dimension, DiPixelFormat fmt )
    {
        uint32 blockDimension = 0;
        if(IsCompressedFormat(fmt))
        {
            blockDimension = dimension / 4;
            if(dimension % 4) blockDimension++;
        }
        else
        {
            blockDimension = dimension;
        }
        return blockDimension;
    }

    uint32 DiPixelBox::GetFormatBlockSize( DiPixelFormat fmt )
    {
        return ComputeImageByteSize(1, 1, fmt);
    }
    static inline unsigned int FixedToFixed(uint32 value, unsigned int n, unsigned int p) 
    {
        if(n > p) 
        {
            value >>= n-p;
        } 
        else if(n < p)
        {
            if(value == 0)
                value = 0;
            else if(value == (static_cast<unsigned int>(1)<<n)-1)
                value = (1<<p)-1;
            else
                value = value*(1<<p)/((1<<n)-1);
        }
        return value;    
    }

    static inline unsigned int FloatToFixed(const float value, const unsigned int bits)
    {
        if(value <= 0.0f) return 0;
        else if (value >= 1.0f) return (1<<bits)-1;
        else return (unsigned int)(value * (1<<bits));     
    }

    static inline float FixedToFloat(unsigned value, unsigned int bits)
    {
        return (float)value/(float)((1<<bits)-1);
    }

    static inline unsigned int IntRead(const void *src, int n) 
    {
        switch(n) 
        {
        case 1:
            return ((uint8*)src)[0];
        case 2:
            return ((uint16*)src)[0];
        case 3:
            return ((uint32)((uint8*)src)[0])|
                ((uint32)((uint8*)src)[1]<<8)|
                ((uint32)((uint8*)src)[2]<<16);
        case 4:
            return ((uint32*)src)[0];
        } 
        return 0;
    }

    static inline void IntWrite(void *dest, const int n, const unsigned int value)
    {
        switch(n) 
        {
        case 1:
            ((uint8*)dest)[0] = (uint8)value;
            break;
        case 2:
            ((uint16*)dest)[0] = (uint16)value;
            break;
        case 3:
            ((uint8*)dest)[2] = (uint8)((value >> 16) & 0xFF);
            ((uint8*)dest)[1] = (uint8)((value >> 8) & 0xFF);
            ((uint8*)dest)[0] = (uint8)(value & 0xFF);
            break;
        case 4:
            ((uint32*)dest)[0] = (uint32)value;                
            break;                
        }        
    }

    void DiPixelBox::PackColour( const DiColor &colour, const DiPixelFormat pf, void* dest )
    {
        PackColour(colour.r, colour.g, colour.b, colour.a, pf, dest);
    }

    void DiPixelBox::PackColour( const uint8 r, const uint8 g, const uint8 b, const uint8 a, const DiPixelFormat pf, void* dest )
    {
        PixelFormatDescription des = PixelFormatDescription::GetFormatDesc(pf);
        unsigned int value = ((FixedToFixed(r, 8, des.rbits)<<des.rshift) & des.rmask) |
            ((FixedToFixed(g, 8, des.gbits)<<des.gshift) & des.gmask) |
            ((FixedToFixed(b, 8, des.bbits)<<des.bshift) & des.bmask) |
            ((FixedToFixed(a, 8, des.abits)<<des.ashift) & des.amask);
        IntWrite(dest, des.elemBytes, value);
    }

    void DiPixelBox::PackColour( const float r, const float g, const float b, const float a, const DiPixelFormat pf, void* dest )
    {
        PixelFormatDescription des = PixelFormatDescription::GetFormatDesc(pf);
        const unsigned int value = ((FloatToFixed(r, des.rbits)<<des.rshift) & des.rmask) |
            ((FloatToFixed(g, des.gbits)<<des.gshift) & des.gmask) |
            ((FloatToFixed(b, des.bbits)<<des.bshift) & des.bmask) |
            ((FloatToFixed(a, des.abits)<<des.ashift) & des.amask);
        IntWrite(dest, des.elemBytes, value);
    }

    void DiPixelBox::UnpackColour( DiColor *colour, DiPixelFormat pf, const void* src )
    {
        UnpackColour(&colour->r, &colour->g, &colour->b, &colour->a, pf, src);
    }

    void DiPixelBox::UnpackColour( uint8 *r, uint8 *g, uint8 *b, uint8 *a, DiPixelFormat pf, const void* src )
    {
        PixelFormatDescription des = PixelFormatDescription::GetFormatDesc(pf);
        const unsigned int value = IntRead(src, des.elemBytes);
        if(des.flags & PFF_LUMINANCE)
        {
            *r = *g = *b = (uint8)FixedToFixed(
                (value & des.rmask)>>des.rshift, des.rbits, 8);
        }
        else
        {
            *r = (uint8)FixedToFixed((value & des.rmask)>>des.rshift, des.rbits, 8);
            *g = (uint8)FixedToFixed((value & des.gmask)>>des.gshift, des.gbits, 8);
            *b = (uint8)FixedToFixed((value & des.bmask)>>des.bshift, des.bbits, 8);
        }
        if(des.flags & PFF_HASALPHA)
        {
            *a = (uint8)FixedToFixed((value & des.amask)>>des.ashift, des.abits, 8);
        }
        else
        {
            *a = 255; 
        }
    }

    void DiPixelBox::UnpackColour( float *r, float *g, float *b, float *a, DiPixelFormat pf, const void* src )
    {
        PixelFormatDescription des = PixelFormatDescription::GetFormatDesc(pf);
        const unsigned int value = IntRead(src, des.elemBytes);
        if(des.flags & PFF_LUMINANCE)
        {
            *r = *g = *b = FixedToFloat(
                (value & des.rmask)>>des.rshift, des.rbits);
        }
        else
        {
            *r = FixedToFloat((value & des.rmask)>>des.rshift, des.rbits);
            *g = FixedToFloat((value & des.gmask)>>des.gshift, des.gbits);
            *b = FixedToFloat((value & des.bmask)>>des.bshift, des.bbits);
        }
        if(des.flags & PFF_HASALPHA)
        {
            *a = FixedToFloat((value & des.amask)>>des.ashift, des.abits);
        }
        else
        {
            *a = 1.0f;
        }
    }

    DiPixelBox::DiPixelBox( const DiBox &extents, DiPixelFormat pixelFormat, void *pixelData/*=0*/ ) : DiBox(extents),
        data(pixelData), 
        format(pixelFormat)
    {
        SetConsecutive();
    }

    DiPixelBox::DiPixelBox(uint32 width, uint32 height, DiPixelFormat pixelFormat, void *pixelData/*=0*/) :
        DiBox(0, 0, width, height),
        data(pixelData), format(pixelFormat)
    {
        SetConsecutive();
    }

    uint32 DiPixelBox::GetNumElemBytes( DiPixelFormat format )
    {
        return PixelFormatDescription::GetFormatDesc(format).elemBytes;
    }

    void DiPixelBox::BulkPixelConversion( void *psrc, DiPixelFormat srcFormat, void *dest,
        DiPixelFormat dstFormat, unsigned int count )
    {
        DiPixelBox src(count, 1,srcFormat, psrc);
        DiPixelBox dst(count, 1,dstFormat, dest);

        BulkPixelConversion(src, dst);
    }

    void DiPixelBox::BulkPixelConversion( const DiPixelBox &src, const DiPixelBox &dst )
    {
        DI_ASSERT(src.GetWidth() == dst.GetWidth() &&
            src.GetHeight() == dst.GetHeight() );

        if(DiPixelBox::IsCompressedFormat(src.format) || DiPixelBox::IsCompressedFormat(dst.format))
        {
            if(src.format == dst.format)
            {
                memcpy(dst.data, src.data, src.GetConsecutiveSize());
                return;
            }
            else
            {
                DI_ASSERT("Pixel format doesn't match.");
            }
        }

        if(src.format == dst.format)
        {
            if(src.IsConsecutive() && dst.IsConsecutive())
            {
                memcpy(dst.data, src.data, src.GetConsecutiveSize());
                return;
            }

            const size_t srcPixelSize = DiPixelBox::GetNumElemBytes(src.format);
            const size_t dstPixelSize = DiPixelBox::GetNumElemBytes(dst.format);
            uint8 *srcptr = static_cast<uint8*>(src.data)
                + (src.left + src.top * src.rowPitch) * srcPixelSize;
            uint8 *dstptr = static_cast<uint8*>(dst.data)
                + (dst.left + dst.top * dst.rowPitch) * dstPixelSize;

            const size_t srcRowPitchBytes = src.rowPitch*srcPixelSize;
            const size_t srcSliceSkipBytes = src.GetSliceSkip()*srcPixelSize;

            const size_t dstRowPitchBytes = dst.rowPitch*dstPixelSize;
            const size_t dstSliceSkipBytes = dst.GetSliceSkip()*dstPixelSize;

            const size_t rowSize = src.GetWidth()*srcPixelSize;
            for(size_t y=src.top; y<src.bottom; y++)
            {
                memcpy(dstptr, srcptr, rowSize);
                srcptr += srcRowPitchBytes;
                dstptr += dstRowPitchBytes;
            }
            srcptr += srcSliceSkipBytes;
            dstptr += dstSliceSkipBytes;
            return;
        }

        const size_t srcPixelSize = DiPixelBox::GetNumElemBytes(src.format);
        const size_t dstPixelSize = DiPixelBox::GetNumElemBytes(dst.format);
        uint8 *srcptr = static_cast<uint8*>(src.data)
            + (src.left + src.top * src.rowPitch) * srcPixelSize;
        uint8 *dstptr = static_cast<uint8*>(dst.data)
            + (dst.left + dst.top * dst.rowPitch) * dstPixelSize;

        const size_t srcRowSkipBytes = src.GetRowSkip()*srcPixelSize;
        const size_t srcSliceSkipBytes = src.GetSliceSkip()*srcPixelSize;
        const size_t dstRowSkipBytes = dst.GetRowSkip()*dstPixelSize;
        const size_t dstSliceSkipBytes = dst.GetSliceSkip()*dstPixelSize;

        float r,g,b,a;
        for(size_t y=src.top; y<src.bottom; y++)
        {
            for(size_t x=src.left; x<src.right; x++)
            {
                UnpackColour(&r, &g, &b, &a, src.format, srcptr);
                PackColour(r, g, b, a, dst.format, dstptr);
                srcptr += srcPixelSize;
                dstptr += dstPixelSize;
            }
            srcptr += srcRowSkipBytes;
            dstptr += dstRowSkipBytes;
        }
        srcptr += srcSliceSkipBytes;
        dstptr += dstSliceSkipBytes;
    }

    void DiPixelBox::FormatGetDisplayStr( DiPixelFormat eFormat, char * pszStr, int nBufLen )
    {
        if (eFormat == MAKEFOURCC('N','U','L','L'))
        {
            strncpy( pszStr, "PF_NULL", nBufLen ); 
            return;
        }
        switch( eFormat )
        {
        case PF_R8G8B8         : strncpy( pszStr, "PF_R8G8B8",         nBufLen ); return;
        case PIXEL_FORMAT_MAX  : strncpy( pszStr, "PF_MAX",            nBufLen ); return;
        case PF_A8R8G8B8       : strncpy( pszStr, "PF_A8R8G8B8",       nBufLen ); return;
        case PF_A8B8G8R8       : strncpy( pszStr, "PF_A8B8G8R8",       nBufLen ); return;
        case PF_X8R8G8B8       : strncpy( pszStr, "PF_X8R8G8B8",       nBufLen ); return;
        case PF_A8             : strncpy( pszStr, "PF_A8",             nBufLen ); return;
        case PF_L8             : strncpy( pszStr, "PF_L8",             nBufLen ); return;
        case PF_DXT1           : strncpy( pszStr, "PF_DXT1",           nBufLen ); return;
        case PF_DXT2           : strncpy( pszStr, "PF_DXT2",           nBufLen ); return;
        case PF_DXT3           : strncpy( pszStr, "PF_DXT3",           nBufLen ); return;
        case PF_DXT4           : strncpy( pszStr, "PF_DXT4",           nBufLen ); return;
        case PF_DXT5           : strncpy( pszStr, "PF_DXT5",           nBufLen ); return;
        case PF_R16F           : strncpy( pszStr, "PF_R16F",           nBufLen ); return;
        case PF_G16R16F        : strncpy( pszStr, "PF_G16R16F",        nBufLen ); return;
        case PF_A16B16G16R16F  : strncpy( pszStr, "PF_A16B16G16R16F",  nBufLen ); return;
        case PF_R32F           : strncpy( pszStr, "PF_R32F",           nBufLen ); return;
        case PF_G32R32F        : strncpy( pszStr, "PF_G32R32F",        nBufLen ); return;
        case PF_A32B32G32R32F  : strncpy( pszStr, "PF_A32B32G32R32F",  nBufLen ); return;
        }
        DI_WARNING( "Unsupported pixel format: %d", eFormat );
        return;
    }

    void DiPixelBox::GetBitDepths(DiPixelFormat format, int rgba[4])
    {
        const PixelFormatDescription &des = PixelFormatDescription::GetFormatDesc(format);
        rgba[0] = des.rbits;
        rgba[1] = des.gbits;
        rgba[2] = des.bbits;
        rgba[3] = des.abits;
    }

    DiMap<DiPixelFormat,PixelFormatDescription> PixelFormatDescription::s_kDescs;

    PixelFormatDescription PixelFormatDescription::GetFormatDesc( DiPixelFormat format )
    {
        DiMap<DiPixelFormat,PixelFormatDescription>::iterator it = s_kDescs.find(format);
        if (it != s_kDescs.end())
        {
            return it->second;
        }

        switch(format)
        {
        case PF_R8G8B8:
            {
                PixelFormatDescription desc =
                {
                    3,  
                    PFF_NATIVEENDIAN,
                    PCT_BYTE, 3,
                    8, 8, 8, 0,
                    0xFF0000, 0x00FF00, 0x0000FF, 0,
                    16, 8, 0, 0
                };
                s_kDescs[format] = desc;
                return desc;
            }
            break;
        case PF_A8R8G8B8:
            {
                PixelFormatDescription desc = 
                {
                    4,
                    PFF_HASALPHA | PFF_NATIVEENDIAN,
                    PCT_BYTE, 4,
                    8, 8, 8, 8,
                    0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000,
                    16, 8, 0, 24
                };
                s_kDescs[format] = desc;
                return desc;
            }
            break;
        case PF_A8B8G8R8:
            {
                PixelFormatDescription desc = 
                {
                    4,
                    PFF_HASALPHA | PFF_NATIVEENDIAN,
                    PCT_BYTE, 4,
                    8, 8, 8, 8,
                    0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000,
                    0, 8, 16, 24,
                };
                s_kDescs[format] = desc;
                return desc;
            }
            break;
        case PF_X8R8G8B8:
            {
                PixelFormatDescription desc = 
                {
                    4,
                    PFF_NATIVEENDIAN,
                    PCT_BYTE, 3,
                    8, 8, 8, 0,
                    0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000,
                    16, 8, 0, 24
                };
                s_kDescs[format] = desc;
                return desc;
            }
            break;
        case PF_A8:
            {
                PixelFormatDescription desc = 
                {
                    1,
                    PFF_HASALPHA | PFF_NATIVEENDIAN,
                    PCT_BYTE, 1,
                    0, 0, 0, 8,
                    0, 0, 0, 0xFF, 0, 0, 0, 0
                };
                s_kDescs[format] = desc;
                return desc;
            }
            break;
        case PF_L8:
            {
                PixelFormatDescription desc = 
                {
                    1,
                    PFF_LUMINANCE | PFF_NATIVEENDIAN,
                    PCT_BYTE, 1,
                    8, 0, 0, 0,
                    0xFF, 0, 0, 0, 0, 0, 0, 0
                };
                s_kDescs[format] = desc;
                return desc;
            }
            break;
        case PF_R32F:
            {
                PixelFormatDescription desc = 
                {
                    4,
                    PFF_FLOAT,
                    PCT_FLOAT32, 1,
                    32, 0, 0, 0,
                    0, 0, 0, 0, 0, 0, 0, 0
                };
                s_kDescs[format] = desc;
                return desc;
            }
            break;
        case PF_R16F:
            {
                PixelFormatDescription desc =
                {
                    2,
                    PFF_FLOAT,
                    PCT_FLOAT16, 1,
                    16, 0, 0, 0,
                    0, 0, 0, 0, 0, 0, 0, 0
                };
                s_kDescs[format] = desc;
                return desc;
            }
            break;
        case PF_G16R16F:
            {
                PixelFormatDescription desc =
                {
                    4,
                    PFF_FLOAT,
                    PCT_FLOAT16, 2,
                    16, 16, 0, 0,
                    0, 0, 0, 0, 0, 0, 0, 0
                };
                s_kDescs[format] = desc;
                return desc;
            }
            break;
        case PF_A16B16G16R16F:
            {
                PixelFormatDescription desc =
                {
                    8,
                    PFF_FLOAT | PFF_HASALPHA,
                    PCT_FLOAT16, 4,
                    16, 16, 16, 16,
                    0, 0, 0, 0, 0, 0, 0, 0
                };
                s_kDescs[format] = desc;
                return desc;
            }
            break;
        case PF_G32R32F:
            {
                PixelFormatDescription desc =
                {
                    8,
                    PFF_FLOAT,
                    PCT_FLOAT32, 2,
                    32, 32, 0, 0,
                    0, 0, 0, 0, 0, 0, 0, 0
                };
                s_kDescs[format] = desc;
                return desc;
            }
            break;
        case PF_A32B32G32R32F:
            {
                PixelFormatDescription desc =
                {
                    16,
                    PFF_FLOAT | PFF_HASALPHA,
                    PCT_FLOAT32, 4,
                    32, 32, 32, 32,
                    0, 0, 0, 0, 0, 0, 0, 0
                };
                s_kDescs[format] = desc;
                return desc;
            }
            break;
        case PF_DXT1:
            {
                PixelFormatDescription desc = 
                {
                    0,
                    PFF_COMPRESSED | PFF_HASALPHA,
                    PCT_BYTE, 3, 
                    0, 0, 0, 0,
                    0, 0, 0, 0, 0, 0, 0, 0
                };
                s_kDescs[format] = desc;
                return desc;
            }
            break;
        case PF_DXT2:
            {
                PixelFormatDescription desc = 
                {
                    0,
                    PFF_COMPRESSED | PFF_HASALPHA,
                    PCT_BYTE, 4,
                    0, 0, 0, 0,
                    0, 0, 0, 0, 0, 0, 0, 0
                };
                s_kDescs[format] = desc;
                return desc;
            }
            break;
        case PF_DXT3:
            {
                PixelFormatDescription desc = 
                {
                    0,
                    PFF_COMPRESSED | PFF_HASALPHA,
                    PCT_BYTE, 4,
                    0, 0, 0, 0,
                    0, 0, 0, 0, 0, 0, 0, 0
                };
                s_kDescs[format] = desc;
                return desc;
            }
            break;
        case PF_DXT4:
            {
                PixelFormatDescription desc = 
                {
                    0,
                    PFF_COMPRESSED | PFF_HASALPHA,
                    PCT_BYTE, 4,
                    0, 0, 0, 0,
                    0, 0, 0, 0, 0, 0, 0, 0
                };
                s_kDescs[format] = desc;
                return desc;
            }
            break;
        case PF_DXT5:
            {
                PixelFormatDescription desc = 
                {
                    0,
                    PFF_COMPRESSED | PFF_HASALPHA,
                    PCT_BYTE, 4,
                    0, 0, 0, 0,
                    0, 0, 0, 0, 0, 0, 0, 0
                };
                s_kDescs[format] = desc;
                return desc;
            }
            break;
        default:
            DI_WARNING("Unsupported pixel format: %d", format);
            break;
        }
        return PixelFormatDescription();
    }
}