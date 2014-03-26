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
#include "K2RenderObjects.h"
#include "K2Game.h"
#include "K2Clip.h"

#include "CullNode.h"
#include "GfxDriver.h"
#include "SceneManager.h"

namespace Demi
{
    DiK2RenderObject::DiK2RenderObject()
        : mNode(nullptr)
        , mVisible(true)
        , mRotRadian(0)
        , mScale(DiVec3::UNIT_SCALE)
    {
    }

    DiK2RenderObject::~DiK2RenderObject()
    {
        Release();
    }

    void DiK2RenderObject::Release()
    {
        mModel.reset();
    }

    void DiK2RenderObject::SetVisible(bool vis)
    {
        mVisible = vis;
        mNode->SetVisible(vis);
    }

    void DiK2RenderObject::SetRotation(const float rotRad)
    {
        mRotation.FromAngleAxis(DiRadian(rotRad), DiVec3::UNIT_Y);
        mNode->SetOrientation(mRotRadian);
    }

    void DiK2RenderObject::SetRotation(const DiQuat& quat)
    {
        mRotation = quat;
        mRotRadian = mRotation.getYaw().valueRadians();
        mNode->SetOrientation(mRotation);
    }

    void DiK2RenderObject::SetWorldPosition(const DiVec3& pos)
    {
        mNode->SetPosition(pos);
    }

    void DiK2RenderObject::SetPosition(const DiK2Pos& pos)
    {
        // TODO
    }

    void DiK2RenderObject::SetScale(const DiVec3& scale)
    {
        mScale = scale;
        mNode->SetScale(scale);
    }

}