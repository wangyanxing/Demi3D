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

#include "DrvGLES2Pch.h"
#include "ES2TypeMappings.h"
#include "VertexFormat.h"
#include "Texture.h"
#include "ShaderParam.h"
#include "Capabilities.h"
#include "ES2FrameBuffer.h"
#include "Image.h"

namespace Demi
{
    GLenum DiGLTypeMappings::GLAddressMode[] =
    {
        GL_REPEAT,
        GL_MIRRORED_REPEAT,
        GL_CLAMP_TO_EDGE,
#ifndef GL_ES_VERSION_2_0
        GL_CLAMP,
#else
        GL_CLAMP_TO_EDGE,
#endif
        GL_NONE
    };

    GLenum DiGLTypeMappings::GetGLUsage(uint32 usage)
    {
        switch (usage)
        {
        case RU_STATIC:
            return GL_STATIC_DRAW;
        case RU_DYNAMIC:
            return GL_DYNAMIC_DRAW;
        case RU_WRITE_ONLY:
            return GL_STREAM_DRAW;
        default:
            return GL_DYNAMIC_DRAW;
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
        case VERT_USAGE_SECONDARY_COLOR:
            return 4;
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
        GL_RGB,                            // PF_R8G8B8,
        GL_BGRA,                           // PF_A8R8G8B8,
        GL_BGRA,                           // PF_A8B8G8R8,
        GL_BGRA,                           // PF_X8R8G8B8,
        GL_ALPHA,                          // PF_A8,
        GL_LUMINANCE,                      // PF_L8,

#if GL_EXT_texture_compression_dxt1
        GL_COMPRESSED_RGBA_S3TC_DXT1_EXT,  // PF_DXT1,
#else
        GL_NONE,
#endif
        GL_NONE,                           // PF_DXT2,
#if GL_EXT_texture_compression_s3tc
        GL_COMPRESSED_RGBA_S3TC_DXT3_EXT,  // PF_DXT3,
#else
        GL_NONE,
#endif
        GL_NONE,                           // PF_DXT4,
#if GL_EXT_texture_compression_s3tc
        GL_COMPRESSED_RGBA_S3TC_DXT5_EXT,  // PF_DXT5,
#else
        GL_NONE,
#endif

#if GL_EXT_texture_rg
        GL_LUMINANCE,                      // PF_R16F,
#else
        GL_NONE,
#endif

#if GL_EXT_texture_rg
        GL_RG_EXT,                         // PF_G16R16F,
#else
        GL_NONE,
#endif

#if GL_OES_texture_half_float || GL_EXT_color_buffer_half_float
        GL_RGBA,                           // PF_A16B16G16R16F,
#else
        GL_NONE,
#endif

#if GL_EXT_texture_rg
        GL_LUMINANCE,                      // PF_R32F,
#else
        GL_NONE,
#endif

#if GL_EXT_texture_rg
        GL_RG_EXT,                         // PF_G32R32F,
#else
        GL_NONE,
#endif

#if GL_OES_texture_half_float || GL_EXT_color_buffer_half_float
        GL_RGBA,                           // PF_A32B32G32R32F,
#else
        GL_NONE,
#endif

#if GL_IMG_texture_compression_pvrtc
        GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG,  // PF_PVRTC_RGB2
        GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG, // PF_PVRTC_RGBA2
        GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG,  // PF_PVRTC_RGB4
        GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG, // PF_PVRTC_RGBA4
#else
        GL_NONE,
        GL_NONE,
        GL_NONE,
        GL_NONE,
        GL_NONE,
#endif

#if GL_IMG_texture_compression_pvrtc2
        GL_COMPRESSED_RGBA_PVRTC_2BPPV2_IMG, // PF_PVRTC2_2BPP
        GL_COMPRESSED_RGBA_PVRTC_4BPPV2_IMG, // PF_PVRTC2_4BPP
#else
        GL_NONE,
        GL_NONE,
#endif

#ifdef GL_OES_compressed_ETC1_RGB8_texture
        GL_ETC1_RGB8_OES,   // PF_ETC1_RGB8,
        GL_NONE,            // PF_ETC2_RGB8,
        GL_NONE,            // PF_ETC2_RGBA8,
        GL_NONE,            // PF_ETC2_RGB8A1,
#else
        GL_NONE,
        GL_NONE,
        GL_NONE,
        GL_NONE,
#endif

#ifdef GL_AMD_compressed_ATC_texture
        ATC_RGB_AMD, // PF_ATC_RGB,
        ATC_RGBA_EXPLICIT_ALPHA_AMD, // PF_ATC_RGBA_EXPLICIT_ALPHA,
        ATC_RGBA_INTERPOLATED_ALPHA_AMD, // PF_ATC_RGBA_INTERPOLATED_ALPHA,
#else
        GL_NONE,
        GL_NONE,
        GL_NONE
#endif
    };

    GLenum DiGLTypeMappings::GLInternalFormatMapping[PIXEL_FORMAT_MAX] =
    {
        GL_RGB8,                           // PF_R8G8B8,
        GL_RGBA8,                          // PF_A8R8G8B8,
        GL_RGBA8,                          // PF_A8B8G8R8,
        GL_RGB8,                           // PF_X8R8G8B8,
        GL_ALPHA,                          // PF_A8,
        GL_LUMINANCE,                      // PF_L8,
#if GL_EXT_texture_compression_dxt1
        GL_COMPRESSED_RGBA_S3TC_DXT1_EXT,  // PF_DXT1,
#else
        GL_NONE,
#endif
        GL_NONE,                           // PF_DXT2,
#if GL_EXT_texture_compression_s3tc
        GL_COMPRESSED_RGBA_S3TC_DXT3_EXT,  // PF_DXT3,
#else
        GL_NONE,
#endif
        GL_NONE,                           // PF_DXT4,
#if GL_EXT_texture_compression_s3tc
        GL_COMPRESSED_RGBA_S3TC_DXT5_EXT,  // PF_DXT5,
#else
        GL_NONE,
#endif
        GL_R16F_EXT,                       // PF_R16F,
        GL_RG16F_EXT,                      // PF_G16R16F,
        GL_NONE,                           // PF_A16B16G16R16F,
        GL_R32F_EXT,                       // PF_R32F,
        GL_RG32F_EXT,                      // PF_G32R32F,
        GL_NONE,                           // PF_A32B32G32R32F,

#if GL_IMG_texture_compression_pvrtc
        GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG,  // PF_PVRTC_RGB2
        GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG, // PF_PVRTC_RGBA2
        GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG,  // PF_PVRTC_RGB4
        GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG, // PF_PVRTC_RGBA4
#else
        GL_NONE,
        GL_NONE,
        GL_NONE,
        GL_NONE,
        GL_NONE,
#endif

#if GL_IMG_texture_compression_pvrtc2
        GL_COMPRESSED_RGBA_PVRTC_2BPPV2_IMG, // PF_PVRTC2_2BPP
        GL_COMPRESSED_RGBA_PVRTC_4BPPV2_IMG, // PF_PVRTC2_4BPP
#else
        GL_NONE,
        GL_NONE,
#endif

#ifdef GL_OES_compressed_ETC1_RGB8_texture
        GL_ETC1_RGB8_OES,   // PF_ETC1_RGB8,
        GL_NONE,            // PF_ETC2_RGB8,
        GL_NONE,            // PF_ETC2_RGBA8,
        GL_NONE,            // PF_ETC2_RGB8A1,
#else
        GL_NONE,
        GL_NONE,
        GL_NONE,
        GL_NONE,
#endif

#ifdef GL_AMD_compressed_ATC_texture
        ATC_RGB_AMD, // PF_ATC_RGB,
        ATC_RGBA_EXPLICIT_ALPHA_AMD, // PF_ATC_RGBA_EXPLICIT_ALPHA,
        ATC_RGBA_INTERPOLATED_ALPHA_AMD, // PF_ATC_RGBA_INTERPOLATED_ALPHA,
#else
        GL_NONE,
        GL_NONE,
        GL_NONE
#endif

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
            DI_WARNING("Unsupported gles texture type");
            return GL_TEXTURE_2D;
        }
    }

    DiShaderParameter::ParamType DiGLTypeMappings::ConvertGLShaderConstType(GLenum gltype)
    {
        switch (gltype)
        {
        case GL_FLOAT:
            return DiShaderParameter::VARIABLE_FLOAT;
        case GL_FLOAT_VEC2:
            return DiShaderParameter::VARIABLE_FLOAT2;
        case GL_FLOAT_VEC3:
            return DiShaderParameter::VARIABLE_FLOAT3;
        case GL_FLOAT_VEC4:
            return DiShaderParameter::VARIABLE_FLOAT4;
        case GL_FLOAT_MAT4:
            return DiShaderParameter::VARIABLE_MAT4;
        case GL_SAMPLER_2D:
        case GL_SAMPLER_2D_RECT_ARB:
            return DiShaderParameter::VARIABLE_SAMPLER2D;
        case GL_SAMPLER_CUBE:
            return DiShaderParameter::VARIABLE_SAMPLERCUBE;
        default:
            return DiShaderParameter::NUM_VARIABLE_TYPES;
        }
    }

    GLenum DiGLTypeMappings::GetExternalFormat(GLenum format)
    {
        return format;
    }

    GLenum DiGLTypeMappings::GetDataType(DiPixelFormat format)
    {
        switch (format)
        {
        case PF_A8:
        case PF_L8:
        case PF_R8G8B8:
            return GL_UNSIGNED_BYTE;

#if DEMI_ENDIAN == DEMI_BIG_ENDIAN
        case PF_X8R8G8B8:
        case PF_A8B8G8R8:
        case PF_A8R8G8B8:
            return GL_UNSIGNED_INT_8_8_8_8_REV;
#else
        case PF_A8B8G8R8:
        case PF_X8R8G8B8:
        case PF_A8R8G8B8:
            return GL_UNSIGNED_BYTE;
#endif
        case PF_R16F:
        case PF_G16R16F:
        case PF_A16B16G16R16F:
#if (GL_OES_texture_half_float && OGRE_PLATFORM != OGRE_PLATFORM_NACL) || (OGRE_NO_GLES3_SUPPORT == 0)
            return GL_HALF_FLOAT_OES;
#else
            return 0;
#endif
        case PF_R32F:
        case PF_G32R32F:
        case PF_A32B32G32R32F:
#if (GL_OES_texture_float && OGRE_PLATFORM != OGRE_PLATFORM_NACL) || (OGRE_NO_GLES3_SUPPORT == 0)
            return GL_FLOAT;
#endif
        default:
            return 0;
        }
    }

    GLenum DiGLTypeMappings::GetClosestGLInternalFormat(DiPixelFormat fmt, bool hwGamma /*= false*/)
    {
        GLenum format = GLInternalFormatMapping[fmt];
        if (format == GL_NONE)
        {
            if (hwGamma)
                return 0; // not supported
            else
                return GL_RGBA8_OES;
        }
        else
            return format;
    }

    uint32 DiGLTypeMappings::OptionalPO2(uint32 value)
    {
        if (Driver->GetGfxCaps()->hasCapability(RSC_NON_POWER_OF_2_TEXTURES))
            return value;
        else
            return DiMath::FirstPO2From(value);
    }

    DiPixelFormat DiGLTypeMappings::GetNativeFormat(DiTextureType ttype, 
        DiPixelFormat format, int usage)
    {
        // Check compressed texture support
        // if a compressed format not supported, revert to PF_A8R8G8B8
        if (DiPixelBox::IsCompressedFormat(format) &&
            !Driver->GetGfxCaps()->hasCapability(RSC_TEXTURE_COMPRESSION_DXT) &&
            !Driver->GetGfxCaps()->hasCapability(RSC_TEXTURE_COMPRESSION_PVRTC) &&
            !Driver->GetGfxCaps()->hasCapability(RSC_TEXTURE_COMPRESSION_ATC) &&
            !Driver->GetGfxCaps()->hasCapability(RSC_TEXTURE_COMPRESSION_ETC1))
        {
            return PF_A8R8G8B8;
        }
        // if floating point textures not supported, revert to PF_A8R8G8B8
        if (DiPixelBox::IsFloatingPoint(format) &&
            !Driver->GetGfxCaps()->hasCapability(RSC_TEXTURE_FLOAT))
        {
            return PF_A8R8G8B8;
        }

        // Check if this is a valid rendertarget format
        if (usage & TU_RENDER_TARGET)
        {
            /// Get closest supported alternative
            /// If mFormat is supported it's returned
            return DiGLES2Driver::FBOManager->GetSupportedAlternative(format);
        }

        // Supported
        return format;
    }

    uint32 DiGLTypeMappings::GetMaxMipmaps(uint32 width, uint32 height, uint32 depth, DiPixelFormat format)
    {
        uint32 count = 0;
        if ((width > 0) && (height > 0) && (depth > 0))
        {
            do {
                if (width > 1)
                    width = width / 2;
                if (height > 1)
                    height = height / 2;
                if (depth > 1)
                    depth = depth / 2;
                /*
                NOT needed, compressed formats will have mipmaps up to 1x1
                if(PixelUtil::isValidExtent(width, height, depth, format))
                count ++;
                else
                break;
                */

                count++;
            } while (!(width == 1 && height == 1 && depth == 1));
        }
        return count;
    }

}