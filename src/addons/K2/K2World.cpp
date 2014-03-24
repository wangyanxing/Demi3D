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
#include "SceneManager.h"
#include "CullNode.h"

#include "K2TerrainMap.h"
#include "K2World.h"
#include "K2WorldSerial.h"
#include "K2Model.h"

namespace Demi
{
    DiK2World::DiK2World(DiSceneManager* sm)
        : mRootNode(nullptr)
    {
        mRootNode = sm->GetRootNode()->CreateChild();
    }

    DiK2World::~DiK2World()
    {
        Unload();
    }

    void DiK2World::Unload()
    {
        for (auto i = mModels.begin(); i != mModels.end(); ++i)
        {
            DI_DELETE (*i);
        }
        mModels.clear();

        mTerrain->Unload();
    }

    void DiK2World::Load(const DiString& path)
    {
        DiK2WorldSerial serial;
        serial.Load(path,this);
        mRootNode->AttachObject(mTerrain);
    }

    DiK2Model* DiK2World::AddModel(const DiString& mdf, const DiString& type)
    {
        DiK2Model* md = DI_NEW DiK2Model(mdf);
        md->CreateNode(mRootNode);
        mModels.push_back(md);
        return md;
    }
}