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
#include "AssetManager.h"
#include "Command.h"
#include "K2Asset.h"

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

        // default hon media folder
        if (!DiBase::CommandMgr->HasCommand("k2_media_folder"))
        {
            DiString honMediaPath = DiAssetManager::GetInstance().GetBasePath();
            honMediaPath += "../media_hon"; // default value
            honMediaPath.SimplifyPath();
            DiBase::CommandMgr->RegisterString("k2_media_folder", honMediaPath, 0);
        }

        // register the asset type
        DiAssetManager::GetInstance().RegisterAssetType(DiK2ModelAsset::TYPE, "", [](const DiString& name){
            return make_shared<DiK2ModelAsset>(name);
        });
    }

    void DiK2Plugin::Uninstall()
    {
        DiAssetManager::GetInstance().UnregisterAssetType(DiK2ModelAsset::TYPE);

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