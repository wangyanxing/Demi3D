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
#include "K2Clip.h"
#include "K2Model.h"
#include "K2World.h"
#include "K2Terrain.h"

#include "DebugHelper.h"
#include "CullNode.h"
#include "GfxDriver.h"
#include "SceneManager.h"
#include "ShaderManager.h"

//#define _VISUALIZE_AABB

namespace Demi
{
    DiK2RenderObject::DiK2RenderObject(DiK2World* world)
        : mWorld(world)
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
    
    DiVec3 DiK2RenderObject::GetWorldPosition()
    {
        return mNode->GetPosition();
    }

    void DiK2RenderObject::SetWorldPosition(const DiVec3& pos)
    {
        mPosition.FromWorldPos(pos);
        mNode->SetPosition(pos);
    }
    
    void DiK2RenderObject::SetPosition(const DiK2Pos& pos, float height)
    {
        auto worldPos = pos.ToWorldPos();
        worldPos.y = height;
        mNode->SetPosition(worldPos);
        mPosition = pos;
    }

    void DiK2RenderObject::SetPosition(const DiK2Pos& pos, bool autoHeight)
    {
        auto worldPos = pos.ToWorldPos();
        if(autoHeight)
        {
            auto terrain = mWorld->GetTerrain();
            float outHeight = 0;
            if(terrain->GetHeight(worldPos.x,worldPos.z,outHeight))
            {
                worldPos.y = outHeight;
            }
        }
        mNode->SetPosition(worldPos);
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

    void DiK2RenderObject::Update(float dt)
    {
    }

    void DiK2RenderObject::PostInit()
    {
        if(mModel)
        {
            mModel->SetManualRayQueryFunc([this](const DiRay& ray){
                DiAABB b = mSelectBounds;
                b.mMaximum += GetWorldPosition();
                b.mMinimum += GetWorldPosition();
                return ray.intersects(b);
            });
        }
        
#ifdef _VISUALIZE_AABB
        mDebugger = make_shared<DiDebugHelper>();
        DiMaterialPtr helpermat = DiMaterial::QuickCreate(
            "basic_v", "basic_p", SHADER_FLAG_USE_COLOR);
        helpermat->SetDepthCheck(false);
        mDebugger->SetMaterial(helpermat);
        mNode->AttachObject(mDebugger);
#endif
    }
    
    void DiK2RenderObject::SetSelectionBounds(float radius, float height)
    {
        mSelectBounds.SetNull();
        mSelectBounds.Merge(DiVec3(-radius,0,-radius));
        mSelectBounds.Merge(DiVec3(radius,height,radius));
    }

    float DiK2RenderObject::GetRadius()
    {
        auto half = mSelectBounds.GetHalfSize();
        float rad = DiMath::Max(half.x, half.z);
        return DiK2Pos::FromWorldScale(rad);
    }
}