
#include "GLTypeMappings.h"

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
        return 0;
    }
}