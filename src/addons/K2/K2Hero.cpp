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
#include "K2Hero.h"
#include "K2Game.h"
#include "K2Clip.h"

#include "CullNode.h"
#include "GfxDriver.h"
#include "SceneManager.h"

namespace Demi
{
    DiK2Hero::DiK2Hero(DiK2Game* game)
        : mNode(nullptr)
        , mGame(game)
    {
    }

    DiK2Hero::~DiK2Hero()
    {
        Release();
    }

    DiK2ModelPtr DiK2Hero::LoadModel(const DiString& mdf)
    {
        DiSceneManager* sm = Driver->GetSceneManager();
        mNode = sm->GetRootNode()->CreateChild();

        mModel = make_shared<DiK2Model>(mdf);
        mModel->GetAnimation()->Play("idle");
        mNode->AttachObject(mModel);

        return mModel;
    }

    void DiK2Hero::Release()
    {
    }

}