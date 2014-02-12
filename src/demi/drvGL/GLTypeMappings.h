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

        static GLenum   GLInternalFormatMapping[PIXEL_FORMAT_MAX];

        static GLenum   GLAddressMode[AM_MAX];

        static GLenum   GetClosestGLInternalFormat(DiPixelFormat fmt, bool hwGamma = false);

        static GLenum   GetGLTextureType(uint16 type);

        static GLenum   GetExternalFormat(GLenum format);

        static GLenum   GetDataType(GLenum format);

        static DiShaderParameter::ParamType   ConvertGLShaderConstType(GLenum gltype);
    };
}