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

        Init();
    }
    
    DiScriptManager::~DiScriptManager()
    {
        Shutdown();
        sMgr = nullptr;
    }
    
    void DiScriptManager::Init()
    {
        DI_LOG("Initializing lua script module...");
        mLuaState = luaL_newstate();
        luaL_openlibs(mLuaState);
    }
    
    void DiScriptManager::Shutdown()
    {
        DI_LOG("Shuting down lua script module...");
        lua_close(mLuaState);
        mLuaState = nullptr;
    }
}