
#include "GLTypeMappings.h"
#include "VertexFormat.h"
#include "Texture.h"

namespace Demi
{
    GLenum DiGLTypeMappings::GetGLUsage(uint32 usage)
    {
        switch (usage)
        {
        case RU_STATIC:
            return GL_STATIC_DRAW_ARB;
        case RU_DYNAMIC:
            return GL_DYNAMIC_DRAW_ARB;
        case RU_WRITE_ONLY:
            return GL_STREAM_DRAW_ARB;
        default:
            return GL_DYNAMIC_DRAW_ARB;
        };
    }

    GLenum DiGLTypeMappings::GetGLType(uint32 type)
    {
        switch (type)
        {
        case VERT_TYPE_FLOAT1:
        case VERT_TYPE_FLOAT2:
        case VERT_TYPE_FLOAT3:
        case VERT_TYPE_FLOAT4:
            return GL_FLOAT;
        case VERT_TYPE_SHORT2:
        case VERT_TYPE_SHORT4:
            return GL_SHORT;
        case VERT_TYPE_COLOR:
        case VERT_TYPE_UBYTE4:
            return GL_UNSIGNED_BYTE;
        default:
            return 0;
        };
    }

    GLuint DiGLTypeMappings::GetFixedAttributeIndex(uint8 semantic, uint8 index)
    {
        // Some drivers (e.g. OS X on nvidia) incorrectly determine the attribute binding automatically
        // and end up aliasing existing built-ins. So avoid! Fixed builtins are: 

        //  a  builtin				custom attrib name
        // ----------------------------------------------
        //	0  gl_Vertex			vertex
        //  1  n/a					blendWeights		
        //	2  gl_Normal			normal
        //	3  gl_Color				colour
        //	4  gl_SecondaryColor	secondary_colour
        //	5  gl_FogCoord			fog_coord
        //  7  n/a					blendIndices
        //	8  gl_MultiTexCoord0	uv0
        //	9  gl_MultiTexCoord1	uv1
        //	10 gl_MultiTexCoord2	uv2
        //	11 gl_MultiTexCoord3	uv3
        //	12 gl_MultiTexCoord4	uv4
        //	13 gl_MultiTexCoord5	uv5
        //	14 gl_MultiTexCoord6	uv6, tangent
        //	15 gl_MultiTexCoord7	uv7, binormal

        switch (semantic)
        {
        case VERT_USAGE_POSITION:
            return 0;
        case VERT_USAGE_BLENDWEIGHT:
            return 1;
        case VERT_USAGE_NORMAL:
            return 2;
        case VERT_USAGE_COLOR:
            return 3;
        case VERT_USAGE_BLENDINDICES:
            return 7;
        case VERT_USAGE_TEXCOORD:
            return 8 + index;
        case VERT_USAGE_TANGENT:
            return 14;
        case VERT_USAGE_BINORMAL:
            return 15;
        default:
            DI_ASSERT(false && "Missing attribute!");
            return 0;
        };
    }

    GLenum DiGLTypeMappings::GLFormatMapping[PIXEL_FORMAT_MAX] =
    {
        GL_RGB8,                           // PF_R8G8B8,
        GL_RGBA8,                          // PF_A8R8G8B8,
        GL_RGBA8,                          // PF_A8B8G8R8,
        GL_RGB8,                           // PF_X8R8G8B8,
        GL_ALPHA8,                         // PF_A8,
        GL_LUMINANCE8,                     // PF_L8,
        GL_COMPRESSED_RGBA_S3TC_DXT1_EXT,  // PF_DXT1,
        GL_NONE,                           // PF_DXT2,
        GL_COMPRESSED_RGBA_S3TC_DXT3_EXT,  // PF_DXT3,
        GL_NONE,                           // PF_DXT4,
        GL_COMPRESSED_RGBA_S3TC_DXT5_EXT,  // PF_DXT5,
        GL_LUMINANCE16F_ARB,               // PF_R16F,
        GL_LUMINANCE_ALPHA16F_ARB,         // PF_G16R16F,
        GL_RGBA16F_ARB,                    // PF_A16B16G16R16F,
        GL_LUMINANCE32F_ARB,               // PF_R32F,
        GL_LUMINANCE_ALPHA32F_ARB,         // PF_G32R32F,
        GL_RGBA32F_ARB,                    // PF_A32B32G32R32F,
    };

    GLenum DiGLTypeMappings::GetGLTextureType(uint16 type)
    {
        switch (type)
        {
        case TEXTURE_2D:
            return GL_TEXTURE_2D;
        case TEXTURE_CUBE:
            return GL_TEXTURE_CUBE_MAP;
        default:
            return GL_TEXTURE_2D;
        }
    }

}