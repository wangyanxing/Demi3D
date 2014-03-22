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

namespace Demi
{
    DiK2Plugin* plugin = nullptr;

    const DiString& DiK2Plugin::GetName() const
    {
        static DiString name = "K2Plugin";
        return name;
    }

    void DiK2Plugin::Install()
    {
        DI_LOG("K2 Plugin loaded");

        DiK2Configs::Init();
    }

    void DiK2Plugin::Uninstall()
    {
        DiK2Configs::Shutdown();

        DI_LOG("K2 Plugin unloaded");
    }

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
}