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

#include "K2Pch.h"
#include "K2Plugin.h"
#include "K2Configs.h"
#include "EffectManager.h"

namespace Demi
{
    const DiString& DiK2Plugin::GetName() const
    {
        static DiString name = "K2Plugin";
        return name;
    }

    void DiK2Plugin::Install()
    {
        DI_LOG("K2 Plugin loaded");

        DiK2Configs::Init();
        DI_NEW DiEffectManager();
    }

    void DiK2Plugin::Uninstall()
    {
        DI_DELETE DiEffectManager::GetInstancePtr();
        DiK2Configs::Shutdown();

        DI_LOG("K2 Plugin unloaded");
    }

#ifndef DEMI_STATIC_API
    DiK2Plugin* plugin = nullptr;
    extern "C" void DEMI_K2_API PluginBegin() throw()
    {
        plugin = DI_NEW DiK2Plugin();
        plugin->Install();
    }

    extern "C" void DEMI_K2_API PluginEnd()
    {
        plugin->Uninstall();
        DI_DELETE plugin;
    }
#endif
}