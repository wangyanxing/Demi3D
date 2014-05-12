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
#include "ArenaMotionProperty.h"
#include "ArenaStateProperty.h"
#include "K2Terrain.h"
#include "K2World.h"
#include "ArenaGameApp.h"
#include "ArenaGameEntity.h"
#include "K2RenderObjects.h"

namespace Demi
{
    ArMotionProperty::ArMotionProperty()
    {
    }

    ArMotionProperty::~ArMotionProperty()
    {
    }

    void ArMotionProperty::Update(float dt)
    {
        ArProperty::Update(dt);
    }

    void ArMotionProperty::ModalityChange(ArModalityType mod)
    {  
        if (mModality == mod)
            return;

        DiK2RenderObject* obj = mEntity->GetRenderObj();
        DI_ASSERT(obj);
        
        K2PrefabClip::Clips clip = K2PrefabClip::ANIM_IDLE;
        
        if (mod == MODALITY_STAND)
            clip = K2PrefabClip::ANIM_IDLE;
        else if (mod == MODALITY_WALK)
            clip = K2PrefabClip::ANIM_WALK;

        obj->PlayClip(clip); 

        mModality = mod;
    }

    void ArMotionProperty::PlayClip(K2PrefabClip::Clips clip)
    {
        mModality = MODALITY_INVALID;

        DiK2RenderObject* obj = mEntity->GetRenderObj();
        DI_ASSERT(obj);
        obj->PlayClip(clip);
    }
}