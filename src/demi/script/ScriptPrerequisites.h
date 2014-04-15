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

#ifndef _ScriptPrerequisites_h__
#define _ScriptPrerequisites_h__

#if DEMI_COMPILER == DEMI_COMPILER_MSVC
#   pragma warning(disable : 4996)
#endif

namespace Demi
{
    class DiScriptManager;
}

#include "GfxPch.h"
#include "ScriptDefines.h"

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}


#endif