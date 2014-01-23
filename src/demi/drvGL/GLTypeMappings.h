
/********************************************************************
    File:       GLTypeMappings.h
    Creator:    demiwangya
*********************************************************************/

namespace Demi
{
    class DiGLTypeMappings
    {
    public:

        static GLenum   GetGLUsage(uint32 usage);

        static GLenum   GetGLType(uint32 type);

        static GLuint   GetFixedAttributeIndex(uint8 semantic, uint8 index);

        static GLuint   IsAttributeValid(uint8 semantic);

        static GLenum   GLFormatMapping[PIXEL_FORMAT_MAX];

        static GLenum   GetGLTextureType(uint16 type);
    };
}