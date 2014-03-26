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
#include "K2GameCamera.h"
#include "K2Hero.h"

#include "CullNode.h"

namespace Demi
{
    DiK2GameCamera::DiK2GameCamera(DiCamera* camera)
        : mCamera(camera)
        , mTarget(nullptr)
        , mHeight(200)
    {
    }

    DiK2GameCamera::~DiK2GameCamera()
    {
    }

    void DiK2GameCamera::Update()
    {
        if (!mTarget)
            return;

        DiVec3 pos = mTarget->GetNode()->GetPosition();
        pos.y += mHeight;
        pos.z += mHeight;
        mCamera->SetPosition(pos);
        mCamera->LookAt(mTarget->GetNode()->GetPosition());
    }
}