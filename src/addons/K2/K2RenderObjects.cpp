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
#include "K2Model.h"

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
        mRotRadian = rotRad;
        mRotation.FromAngleAxis(DiRadian(rotRad), DiVec3::UNIT_Y);
        mNode->SetOrientation(mRotation);
    }

    void DiK2RenderObject::SetRotation(const DiQuat& quat)
    {
        mRotation = quat;
        mRotRadian = mRotation.getYaw().valueRadians();
        mNode->SetOrientation(mRotation);
    }

    void DiK2RenderObject::SetWorldPosition(const DiVec3& pos)
    {
        mPosition.FromWorldPos(pos);
        mNode->SetPosition(pos);
    }

    void DiK2RenderObject::SetPosition(const DiK2Pos& pos)
    {
        mNode->SetPosition(pos.ToWorldPos());
        mPosition = pos;
    }

    void DiK2RenderObject::SetScale(const DiVec3& scale)
    {
        mScale = scale;
        mNode->SetScale(scale);
    }

    void DiK2RenderObject::PlayClip(K2PrefabClip::Clips clip)
    {
        mModel->GetAnimation()->Play(clip);
    }

    void DiK2RenderObject::PlayClip(const DiString& clip)
    {
        mModel->GetAnimation()->Play(clip);

    }
}