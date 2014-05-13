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
#include "ArenaAIAttack.h"
#include "ArenaNPCEntity.h"
#include "ArenaAttribute.h"
#include "ArenaEntityConfig.h"
#include "ArenaGameApp.h"
#include "ArenaEntityManager.h"
#include "ArenaMoveProperty.h"

#include "K2RenderObjects.h"

namespace Demi
{
    ArAIAttackCommand::ArAIAttackCommand(ArEntity* entity, ArObjID target)
        : ArAICommand(entity)
        , mTarget(target)
    {
        mType = ENUM_AI_TYPE_ATTACK;
        
        mCoolDownTime = mEntity->GetAttribute()->GetEntityConfig()->attackcooldown / 1000.0f;
        mNumAttackAnims = mEntity->GetAttribute()->GetEntityConfig()->attacknumanims;
    }

    ArAIAttackCommand::~ArAIAttackCommand()
    {
    }

    void ArAIAttackCommand::Enter()
    {
        PlayClip();
    }

    void ArAIAttackCommand::Leave()
    {
    }

    void ArAIAttackCommand::Update(float dt)
    {
        // Update orientation
        auto entityPtr = ArGameApp::Get()->GetEntityManager()->FindEntity(mTarget);
        auto targetEntity = std::dynamic_pointer_cast<ArNPCEntity>(entityPtr);
        if (targetEntity)
        {
            ArMoveProperty::UpdateRotation(mEntity, targetEntity->GetRenderObj()->GetPosition(), dt,
                mEntity->GetAttribute()->GetEntityConfig()->turnrate);
        }

        mTimer += dt;
        if (mTimer >= mCoolDownTime)
        {
            PlayClip();
        }
    }

    void ArAIAttackCommand::Redo()
    {
        PlayClip();
    }

    void ArAIAttackCommand::PlayClip()
    {
        mTimer = 0;

        int animID = 0;
        if (mLastAnimID >= 0)
        {
            animID = DiMath::Random(mNumAttackAnims);
            while (animID == mLastAnimID)
                animID = DiMath::Random(mNumAttackAnims);
        }

        mLastAnimID = animID;

        K2PrefabClip::Clips clip = K2PrefabClip::Clips(K2PrefabClip::ANIM_ATTACK_1 + animID);
        mEntity->GetMotionProperty()->PlayClip(clip);
    }
}