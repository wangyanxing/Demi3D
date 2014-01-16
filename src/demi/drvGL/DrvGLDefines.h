
/********************************************************************
    File:       DrvGLDefines.h
    Creator:    demiwangya
*********************************************************************/

#pragma once

#ifdef DEMI_STATIC_API
#    define DI_GLDRV_API
#else
#    ifdef DI_GL_EXPORT
#        define DI_GLDRV_API  __declspec(dllexport)
#    else
#        define DI_GLDRV_API __declspec(dllimport)
#    endif
#endif