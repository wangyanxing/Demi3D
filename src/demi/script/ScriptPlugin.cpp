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

#include "ScriptPlugin.h"
#include "ScriptManager.h"

namespace Demi
{
    const DiString& DiScriptPlugin::GetName() const
    {
        static DiString plugin = "DrvGL";
        return plugin;
    }

    void DiScriptPlugin::Install()
    {
        DI_NEW DiScriptManager();
    }

    void DiScriptPlugin::Uninstall()
    {
        DI_DELETE DiScriptManager::Get();
    }

    DiScriptPlugin::DiScriptPlugin()
    {
#ifndef DEMI_STATIC_API
#endif
    }

#ifndef DEMI_STATIC_API
    DiScriptPlugin* plugin = nullptr;

    extern "C" void DI_SCRIPT_API PluginBegin() throw()
    {
        plugin = DI_NEW DiScriptPlugin();
        plugin->Install();
    }

    extern "C" void DI_SCRIPT_API PluginEnd()
    {
        plugin->Uninstall();
        DI_DELETE plugin;
    }
#endif
}