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

#include "FxPch.h"
#include "FxPlugin.h"
#include "EffectManager.h"

namespace Demi
{
    const DiString& DiFxPlugin::GetName() const
    {
        static DiString name = "FxPlugin";
        return name;
    }

    void DiFxPlugin::Install()
    {
        DI_LOG("Fx Plugin loaded");
        DI_NEW DiEffectManager();
    }

    void DiFxPlugin::Uninstall()
    {
        DI_LOG("Fx Plugin unloaded");
        DI_DELETE DiEffectManager::GetInstancePtr();
    }

#ifndef DEMI_STATIC_API
    DiFxPlugin* plugin = nullptr;
    extern "C" void DEMI_FX_API PluginBegin() throw()
    {
        plugin = DI_NEW DiFxPlugin();
        plugin->Install();
    }

    extern "C" void DEMI_FX_API PluginEnd()
    {
        plugin->Uninstall();
        DI_DELETE plugin;
    }
#endif
}