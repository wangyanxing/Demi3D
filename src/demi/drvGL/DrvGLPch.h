
/********************************************************************
    File:       DrvGLPch.h
    Creator:    demiwangya
*********************************************************************/

#pragma once

#include "GfxPch.h"

#include "GLPrerequisites.h"
#include "DrvGLDefines.h"

#define WIN32_LEAN_AND_MEAN
#ifndef NOMINMAX
#   define NOMINMAX
#endif

#include <windows.h>
#include <wingdi.h>
#include "gl/glew.h"
#include "gl/wglew.h"
#include "gl/GLU.h"