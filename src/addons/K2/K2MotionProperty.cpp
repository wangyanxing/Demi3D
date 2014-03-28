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
#include "K2MotionProperty.h"
#include "K2StateProperty.h"
#include "K2TerrainMap.h"
#include "K2World.h"
#include "K2GameApp.h"
#include "K2GameEntity.h"
#include "K2RenderObjects.h"

namespace Demi
{
    DiK2MotionProperty::DiK2MotionProperty()
        :mModality(MODALITY_STAND)
    {

    }

    DiK2MotionProperty::~DiK2MotionProperty()
    {

    }

    void DiK2MotionProperty::Update(float dt)
    {
    }

    void DiK2MotionProperty::ModalityChange(K2ModalityType mod)
    {
        if (mModality == mod)
            return;

        DiK2RenderObject* obj = mEntity->GetRenderObj();
        
        K2PrefabClip::Clips clip;
        
        if (mod == MODALITY_STAND)
            clip = K2PrefabClip::ANIM_IDLE;
        else if (mod == MODALITY_WALK)
            clip = K2PrefabClip::ANIM_WALK;

        obj->PlayClip(clip); 

        mModality = mod;
    }

}