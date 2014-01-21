
/********************************************************************
    File:       Image.h
    Creator:    demiwangya
*********************************************************************/

#pragma once

namespace Demi 
{
    class DiImage
    {
    public:

        DiImage(const DiString& imageName);

        DiImage(DiDataStreamPtr stream);

        ~DiImage();

    public:

        BOOL            LoadToTexture(DiTexture* texture);

        int             GetHeight() const { return mHeight; }

        int             GetWidth() const { return mWidth; }

    protected:

        void            ParseTga(DiTexture* texture);

        void            ParseDDS(DiTexture* texture);

    protected:

        int             mWidth;

        int             mHeight;

        DiDataStreamPtr mImageData;
    };

    //////////////////////////////////////////////////////////////////////////

    enum PixelFormatFlags 
    {
        PFF_HASALPHA        = 0x00000001,      
        PFF_COMPRESSED      = 0x00000002,
        PFF_FLOAT           = 0x00000004,         
        PFF_DEPTH           = 0x00000008,
        PFF_NATIVEENDIAN    = 0x00000010,
        PFF_LUMINANCE       = 0x00000020
    };

    enum PixelComponentType
    {
        PCT_BYTE            = 0,   
        PCT_SHORT           = 1,  
        PCT_FLOAT16         = 2,
        PCT_FLOAT32         = 3,
        PCT_COUNT           = 4   
    };

    struct PixelFormatDescription
    {
        static DiMap<DiPixelFormat,PixelFormatDescription> s_kDescs;

        static PixelFormatDescription    GetFormatDesc(DiPixelFormat fmt);

        unsigned char           elemBytes;

        uint32                  flags;

        PixelComponentType      componentType;

        unsigned char           componentCount;

        unsigned char           rbits,gbits,bbits,abits;

        uint32                  rmask, gmask, bmask, amask;

        unsigned char           rshift, gshift, bshift, ashift;
    };

    class DI_GFX_API DiPixelBox : public DiBox
    {
    public:

        DiPixelBox() {}

        virtual         ~DiPixelBox() {}
        
        DiPixelBox(const DiBox& extents, DiPixelFormat pixelFormat, void *pixelData=0);
        
        DiPixelBox(uint32 width, uint32 height, DiPixelFormat pixelFormat, void *pixelData = 0);

    public:

        static uint32    ComputeImageByteSize(uint32 width, uint32 height,DiPixelFormat DiPixelFormat);

        static uint32    GetLevelDimension(uint32 dimension, uint32 level);

        static BOOL      IsCompressedFormat(DiPixelFormat DiPixelFormat);
                         
        static BOOL      IsDepthStencilFormat(DiPixelFormat DiPixelFormat);
                         
        static uint32    GetFormatNumBlocks(uint32 dimension, DiPixelFormat DiPixelFormat);

        static uint32    GetFormatBlockSize(DiPixelFormat DiPixelFormat);

        static uint32    GetNumElemBytes( DiPixelFormat format );

        static void      GetBitDepths(DiPixelFormat format, int rgba[4]);
                         
        static void      PackColour(const DiColor &colour, const DiPixelFormat pf,  void* dest);
                         
        static void      PackColour(const uint8 r, const uint8 g, const uint8 b, const uint8 a, const DiPixelFormat pf,  void* dest);
                         
        static void      PackColour(const float r, const float g, const float b, const float a, const DiPixelFormat pf,  void* dest);
                         
        static void      UnpackColour(DiColor *colour, DiPixelFormat pf,  const void* src);
                         
        static void      UnpackColour(uint8 *r, uint8 *g, uint8 *b, uint8 *a, DiPixelFormat pf,  const void* src);
       
        static void      UnpackColour(float *r, float *g, float *b, float *a, DiPixelFormat pf,  const void* src); 

        static void      BulkPixelConversion(void *src, DiPixelFormat srcFormat, void *dest, DiPixelFormat dstFormat, unsigned int count);

        static void      BulkPixelConversion(const DiPixelBox &src, const DiPixelBox &dst);

        static void      FormatGetDisplayStr( DiPixelFormat eFormat, char * pszStr, int nBufLen );
        
        void*            data;

        DiPixelFormat    format;
      
        size_t           rowPitch;
        
        size_t           slicePitch;
        
        void           SetConsecutive()
        {
            rowPitch = GetWidth();
            slicePitch = GetWidth()*GetHeight();
        }
       
        size_t         GetRowSkip() const { return rowPitch - GetWidth(); }
      
        size_t         GetSliceSkip() const { return slicePitch - (GetHeight() * rowPitch); }
           
        bool           IsConsecutive() const 
        { 
            return rowPitch == GetWidth() && slicePitch == GetWidth()*GetHeight(); 
        }
      
        size_t         GetConsecutiveSize() const;
                       
        DiPixelBox     GetSubVolume(const DiBox &def) const;
        
        DiColor        GetColourAt(size_t x, size_t y, size_t z);

        void           SetColourAt(DiColor const &cv, size_t x, size_t y, size_t z);
    };

}