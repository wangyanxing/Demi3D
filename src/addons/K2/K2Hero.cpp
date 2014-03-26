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

#include "GfxDriver.h"

namespace Demi
{
    DiK2Hero::DiK2Hero(DiK2Game* game)
        : mModel(nullptr)
        , mGame(game)
    {
    }

    DiK2Hero::~DiK2Hero()
    {
        Release();
    }

    DiK2Model* DiK2Hero::LoadModel(const DiString& mdf)
    {
        if (mModel)
            DI_DELETE(mModel);

        DiSceneManager* sm = Driver->GetSceneManager();

        mModel = DI_NEW DiK2Model(mdf);
        mModel->CreateNode(sm);
        mModel->GetAnimation()->Play("idle");
        return mModel;
    }

    void DiK2Hero::Release()
    {
        if (mModel)
            DI_DELETE(mModel);
    }

}