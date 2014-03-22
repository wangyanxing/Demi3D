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

namespace Demi
{
    DiK2World::DiK2World()
        : mRootNode(nullptr)
    {
    }

    DiK2World::~DiK2World()
    {
        Unload();
    }

    void DiK2World::Unload()
    {
        mTerrain->Unload();
    }

    void DiK2World::Load(const DiString& path)
    {
        DiK2WorldSerial serial;
        serial.Load(path,this);
    }

    DiCullNode* DiK2World::CreateNode(DiSceneManager* sm)
    {
        if (mRootNode)
        {
            DI_DELETE mRootNode;
            mRootNode = nullptr;
        }
        mRootNode = sm->GetRootNode()->CreateChild();

        mRootNode->AttachObject(mTerrain);

        return mRootNode;
    }
}