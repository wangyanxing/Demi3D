
/********************************************************************
    File:       nv_dds.h
    Creator:    demiwangya
*********************************************************************/

#ifndef __NV_DDS_H__
#define __NV_DDS_H__

#include <string>
#include <deque>

namespace nv_dds
{
    // surface description flags
    const unsigned DDSF_CAPS   = 0x00000001l;
    const unsigned DDSF_HEIGHT = 0x00000002l;
    const unsigned DDSF_WIDTH  = 0x00000004l;
    const unsigned DDSF_PITCH  = 0x00000008l;
    const unsigned DDSF_PIXELFORMAT = 0x00001000l;
    const unsigned DDSF_MIPMAPCOUNT = 0x00020000l;
    const unsigned DDSF_LINEARSIZE  = 0x00080000l;
    const unsigned DDSF_DEPTH       = 0x00800000l;

    // pixel format flags
    const unsigned DDSF_ALPHAPIXELS    = 0x00000001l;
    const unsigned DDSF_FOURCC         = 0x00000004l;
    const unsigned DDSF_RGB            = 0x00000040l;
    const unsigned DDSF_RGBA           = 0x00000041l;

    // dwCaps1 flags
    const unsigned DDSF_COMPLEX = 0x00000008l;
    const unsigned DDSF_TEXTURE = 0x00001000l;
    const unsigned DDSF_MIPMAP  = 0x00400000l;

    // dwCaps2 flags
    const unsigned DDSF_CUBEMAP            = 0x00000200l;
    const unsigned DDSF_CUBEMAP_POSITIVEX  = 0x00000400l;
    const unsigned DDSF_CUBEMAP_NEGATIVEX  = 0x00000800l;
    const unsigned DDSF_CUBEMAP_POSITIVEY  = 0x00001000l;
    const unsigned DDSF_CUBEMAP_NEGATIVEY  = 0x00002000l;
    const unsigned DDSF_CUBEMAP_POSITIVEZ  = 0x00004000l;
    const unsigned DDSF_CUBEMAP_NEGATIVEZ  = 0x00008000l;
    const unsigned DDSF_CUBEMAP_ALL_FACES  = 0x0000FC00l;
    const unsigned DDSF_VOLUME             = 0x00200000l;

    // compressed texture types
    const unsigned FOURCC_DXT1 = 0x31545844l; //(MAKEFOURCC('D','X','T','1'))
    const unsigned FOURCC_DXT3 = 0x33545844l; //(MAKEFOURCC('D','X','T','3'))
    const unsigned FOURCC_DXT5 = 0x35545844l; //(MAKEFOURCC('D','X','T','5'))

    struct DXTColBlock
    {
        unsigned short col0;
        unsigned short col1;

        unsigned char row[4];
    };

    struct DXT3AlphaBlock
    {
        unsigned short row[4];
    };

    struct DXT5AlphaBlock
    {
        unsigned char alpha0;
        unsigned char alpha1;
        
        unsigned char row[6];
    };

    struct DDS_PIXELFORMAT
    {
        unsigned dwSize;
        unsigned dwFlags;
        unsigned dwFourCC;
        unsigned dwRGBBitCount;
        unsigned dwRBitMask;
        unsigned dwGBitMask;
        unsigned dwBBitMask;
        unsigned dwABitMask;
    };

    struct DDS_HEADER
    {
        unsigned dwSize;
        unsigned dwFlags;
        unsigned dwHeight;
        unsigned dwWidth;
        unsigned dwPitchOrLinearSize;
        unsigned dwDepth;
        unsigned dwMipMapCount;
        unsigned dwReserved1[11];
        DDS_PIXELFORMAT ddspf;
        unsigned dwCaps1;
        unsigned dwCaps2;
        unsigned dwReserved2[3];
    };

    typedef enum TextureType
    {
        TextureNone,
        TextureFlat,    // 1D, 2D, and rectangle textures
        Texture3D,
        TextureCubemap
    };
    
    typedef enum TextureFormat
    {
        TextureUnknown=0,
        TextureBGRA,
        TextureBGR,
        TextureLuminance,
        TextureDXT1,
        TextureDXT3,
        TextureDXT5,
    };

    class CSurface
    {
        public:
            CSurface();
            CSurface(unsigned int w, unsigned int h, unsigned int d, unsigned int imgsize, const unsigned char *pixels);
            CSurface(const CSurface &copy);
            CSurface &operator= (const CSurface &rhs);
            virtual ~CSurface();

            operator unsigned char*() const;

            virtual void create(unsigned int w, unsigned int h, unsigned int d, unsigned int imgsize, const unsigned char *pixels);
            virtual void clear();

            inline unsigned int get_width() const { return m_width; }
            inline unsigned int get_height() const { return m_height; }
            inline unsigned int get_depth() const { return m_depth; }
            inline unsigned int get_size() const { return m_size; }

        private:
            unsigned int m_width;
            unsigned int m_height;
            unsigned int m_depth;
            unsigned int m_size;

            unsigned char *m_pixels;   
    };

    class CTexture : public CSurface
    {
        friend class CDDSImage;

        public:
            CTexture();
            CTexture(unsigned int w, unsigned int h, unsigned int d, unsigned int imgsize, const unsigned char *pixels);
            CTexture(const CTexture &copy);
            CTexture &operator= (const CTexture &rhs);
            ~CTexture();

            void create(unsigned int w, unsigned int h, unsigned int d, unsigned int imgsize, const unsigned char *pixels);
            void clear();

            inline const CSurface &get_mipmap(unsigned int index) const
            { 
                DI_ASSERT(!m_mipmaps.empty());
                DI_ASSERT(index < m_mipmaps.size());
                
                return m_mipmaps[index]; 
            }

            inline void add_mipmap(const CSurface &mipmap)
            {
                m_mipmaps.push_back(mipmap);
            }

            inline unsigned int get_num_mipmaps() const { return (unsigned int)m_mipmaps.size(); }

        protected:
            inline CSurface &get_mipmap(unsigned int index)
            { 
                DI_ASSERT(!m_mipmaps.empty());
                DI_ASSERT(index < m_mipmaps.size());

                return m_mipmaps[index]; 
            }

        private:
            std::deque<CSurface> m_mipmaps;
    };

    class CDDSImage
    {
        public:
            CDDSImage();
            ~CDDSImage();

            void create_textureFlat(TextureFormat format, unsigned int components, const CTexture &baseImage);
            void create_texture3D(TextureFormat format, unsigned int components, const CTexture &baseImage);
            void create_textureCubemap(TextureFormat format, unsigned int components,
                                       const CTexture &positiveX, const CTexture &negativeX, 
                                       const CTexture &positiveY, const CTexture &negativeY, 
                                       const CTexture &positiveZ, const CTexture &negativeZ);

            void clear();
            
            bool load(Demi::DiDataStream* fp, bool flipImage = true);

            bool save(DiString filename, bool flipImage = true);

            bool save(FILE* fp, bool flipImage = true);

            inline operator unsigned char*()
            { 
                DI_ASSERT(m_valid);
                DI_ASSERT(!m_images.empty());
                
                return m_images[0]; 
            }

            inline unsigned int get_width() 
            {
                DI_ASSERT(m_valid);
                DI_ASSERT(!m_images.empty());
                
                return m_images[0].get_width(); 
            }

            inline unsigned int get_height()
            {
                DI_ASSERT(m_valid);
                DI_ASSERT(!m_images.empty());
                
                return m_images[0].get_height(); 
            }

            inline unsigned int get_depth()
            {
                DI_ASSERT(m_valid);
                DI_ASSERT(!m_images.empty());
                
                return m_images[0].get_depth(); 
            }

            inline unsigned int get_size()
            {
                DI_ASSERT(m_valid);
                DI_ASSERT(!m_images.empty());

                return m_images[0].get_size();
            }

            inline unsigned int get_num_mipmaps() 
            { 
                DI_ASSERT(m_valid);
                DI_ASSERT(!m_images.empty());

                return m_images[0].get_num_mipmaps(); 
            }

            inline const CSurface &get_mipmap(unsigned int index, unsigned int face) const
            { 
                DI_ASSERT(m_valid);
                DI_ASSERT(!m_images.empty());
                if (face > 1)
                {
                    DI_ASSERT(m_type == TextureCubemap);
                    DI_ASSERT(face < 6);
                }
                DI_ASSERT(index < m_images[face].get_num_mipmaps());
                return m_images[face].get_mipmap(index); 
            }

            inline const CTexture &get_cubemap_face(unsigned int face) const
            {
                DI_ASSERT(m_valid);
                DI_ASSERT(!m_images.empty());
                DI_ASSERT(m_images.size() == 6);
                DI_ASSERT(m_type == TextureCubemap);
                DI_ASSERT(face < 6);

                return m_images[face];
            }

            inline unsigned int get_components() { return m_components; }
            inline TextureFormat get_format() { return m_format; }
            inline TextureType get_type() { return m_type; }

            inline bool is_compressed() 
            { 
                if ((m_format == TextureDXT1) || 
                    (m_format == TextureDXT3) ||
                    (m_format == TextureDXT5))
                    return true; 
                else
                    return false;
            }
            
            inline bool is_cubemap() { return (m_type == TextureCubemap); }
            inline bool is_volume() { return (m_type == Texture3D); }
            inline bool is_valid() { return m_valid; }

            inline bool is_dword_aligned()
            {
                DI_ASSERT(m_valid);

                int dwordLineSize = get_dword_aligned_linesize(get_width(), m_components*8);
                int curLineSize = get_width() * m_components;
                
                return (dwordLineSize == curLineSize);
            }
            
        private:
            unsigned int clamp_size(unsigned int size);
            unsigned int size_dxtc(unsigned int width, unsigned int height);
            unsigned int size_rgb(unsigned int width, unsigned int height);
            inline void swap_endian(void *val);

            // calculates 4-byte aligned width of image
            inline unsigned int get_dword_aligned_linesize(unsigned int width, unsigned int bpp)
            {
                return ((width * bpp + 31) & -32) >> 3;
            }

            void flip(CSurface &surface);
            void flip_texture(CTexture &texture);

            void swap(void *byte1, void *byte2, unsigned int size);
            void flip_blocks_dxtc1(DXTColBlock *line, unsigned int numBlocks);
            void flip_blocks_dxtc3(DXTColBlock *line, unsigned int numBlocks);
            void flip_blocks_dxtc5(DXTColBlock *line, unsigned int numBlocks);
            void flip_dxt5_alpha(DXT5AlphaBlock *block);

            void write_texture(const CTexture &texture, FILE *fp);
            
            TextureFormat m_format;
            unsigned int m_components;
            TextureType m_type;
            bool m_valid;

            std::deque<CTexture> m_images;
    };
}
#endif
