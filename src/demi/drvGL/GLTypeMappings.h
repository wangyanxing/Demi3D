
/********************************************************************
    File:       GLTypeMappings.h
    Creator:    demiwangya
*********************************************************************/

#include "ShaderParam.h"

namespace Demi
{
    class DiGLTypeMappings
    {
    public:

        static GLenum   GetGLUsage(uint32 usage);

        static GLenum   GetGLType(uint32 type);

        static GLuint   GetFixedAttributeIndex(uint8 semantic, uint8 index);

        static GLenum   GLFormatMapping[PIXEL_FORMAT_MAX];

        static GLenum   GetGLTextureType(uint16 type);

        static DiShaderParameter::ParamType   ConvertGLShaderConstType(GLenum gltype);
    };
}