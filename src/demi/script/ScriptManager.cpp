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

#include "ScriptPch.h"
#include "ScriptManager.h"

namespace Demi
{
    DiScriptManager* DiScriptManager::sMgr = nullptr;
    
    DiScriptManager::DiScriptManager()
        :mLuaState(nullptr)
    {
        DI_ASSERT(!sMgr);
        sMgr = this;
    }
    
    DiScriptManager::~DiScriptManager()
    {
        sMgr = nullptr;
    }
    
    void DiScriptManager::Init()
    {
        mLuaState = luaL_newstate();
        luaL_openlibs(mLuaState);
    }
    
    void DiScriptManager::Shutdown()
    {
        lua_close(mLuaState);
        mLuaState = nullptr;
    }
}