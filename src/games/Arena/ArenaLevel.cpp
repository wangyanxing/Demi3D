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

#include "ArenaPch.h"
#include "ArenaLevel.h"
#include "ArenaConfigsLoader.h"
#include "AssetManager.h"

#include "K2World.h"

#include "XMLFile.h"

namespace Demi
{
    ArLevel::ArLevel()
        :mWorld(nullptr)
    {
    }

    ArLevel::~ArLevel()
    {
        SAFE_DELETE(mWorld);
    }

    void ArLevel::LoadMap(const DiString& configfile)
    {
        // load configs
        auto file = DiAssetManager::GetInstance().OpenArchive(configfile, true);
        if (!file)
            return;
        shared_ptr<DiXMLFile> xmlfile(new DiXMLFile());
        xmlfile->Load(file->GetAsString());
        DiXMLElement root = xmlfile->GetRoot();
        ArConfigMapLoader loader(&mMapConfig);
        loader.Load(root);

        // load map
        SAFE_DELETE(mWorld);
        mWorld = DI_NEW DiK2World();
        mWorld->Load(mMapConfig.path);
    }

    void ArLevel::Update(float dt)
    {
        mWorld->Update(dt);
    }

    DiVec3 ArLevel::GetSpwanPosition()
    {
        DiVec3 out = DiVec3::ZERO;
        mWorld->GetTriggerPosition(mMapConfig.spwanpoint, out);
        return out;
    }
}