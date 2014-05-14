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
#include "ArenaMoveProperty.h"
#include "ArenaMotionProperty.h"
#include "ArenaDynEntity.h"
#include "ArenaStateProperty.h"
#include "K2Terrain.h"
#include "K2World.h"
#include "ArenaGameApp.h"
#include "ArenaGameEntity.h"
#include "K2RenderObjects.h"

namespace Demi
{
    ArMoveProperty::ArMoveProperty()
        : mCurrMoveDistance(0)
        , mNumNode(0)
        , mNumCurTarget(0)
        , mModality(MODALITY_STAND)
        , mWalkMode(ENUM_WALK_MODE_STOP)
        , mTargetDirection(INVALID_INT_VALUE)
        , mTargetPosition(-1, -1)
        , mSpeed(INVALID_FLOAT_VALUE)
        , mTurnSpeed(INVALID_INT_VALUE)
        , mDistance(0)
    {
        // Default value
        mSpeed = 7;
        mTurnSpeed = 500;
    }

    ArMoveProperty::~ArMoveProperty()
    {
    }

    void ArMoveProperty::Update(float dt)
    {
        float moveSpeed = mSpeed;
        int turnSpeed = mTurnSpeed;

        DiK2RenderObject* renderObj = mEntity->GetRenderObj();
        if (!renderObj)
            return;

        if (mWalkMode != ENUM_WALK_MODE_STOP)
        {
            DiK2Pos currentPos = renderObj->GetPosition();
            DiK2Pos targetPos = mPosNode[mNumCurTarget];
            DiK2Pos newPos;

            float fMoveDistance = moveSpeed * dt;
            mCurrMoveDistance += fMoveDistance;
            if (mCurrMoveDistance >= 0.01)
            {
                fMoveDistance = mCurrMoveDistance;
                mCurrMoveDistance = 0;
                float fDistanceToTarget = currentPos.Distance(targetPos);
                while (true)
                {
                    // rest nodes
                    int nLeft = mNumNode - (mNumCurTarget + 1);

                    if (fMoveDistance > fDistanceToTarget)
                    {
                        DiK2Pos oldTargetPos = targetPos;

                        // stop
                        if (nLeft <= 0)
                        {
                            Stop();
                            mNumCurTarget = 0;
                            newPos = targetPos;
                            break;
                        }
                        else
                        {
                            //if (mNumCurTarget > 0 && mNumCurTarget % MAX_SENDPOSREM == 0)
                            //    int nCount = std::min(mNumNode - mNumCurTarget, MAX_SENDPOS);

                            fMoveDistance -= fDistanceToTarget;

                            mNumCurTarget++;

                            targetPos = mPosNode[mNumCurTarget];
                            fDistanceToTarget = oldTargetPos.Distance(targetPos);
                        }
                    }
                    else
                    {
                        if (fDistanceToTarget > 0.0f)
                        {
                            newPos.x = currentPos.x + (fMoveDistance * (targetPos.x - currentPos.x)) / fDistanceToTarget;
                            newPos.z = currentPos.z + (fMoveDistance * (targetPos.z - currentPos.z)) / fDistanceToTarget;
                        }
                        else
                            newPos = targetPos;
                        break;
                    }
                }

                auto& pathFinder = ArGameApp::Get()->GetWorld()->GetTerrain()->GetPathFinder();
                if (!pathFinder.IsReachable(newPos, HeavyPathFinder::BLOCK_LEVEL_WALK))
                {
                    UpdateRotation(mEntity, targetPos, dt, turnSpeed);
                    Stop();
                    mNumCurTarget = 0;
                    return;
                }

#if 0
                if (!pathFinder.IsReachable(newPos, HeavyPathFinder::BLOCK_LEVEL_WALK))
                {
                    int gridx = (unsigned short)(newPos.x);
                    int gridz = (unsigned short)(newPos.z);
                    DI_WARNING("Cannot reach %f %f,%d %d", newPos.x, newPos.z, gridx, gridz);
                }
#endif
                
                renderObj->SetPosition(newPos);

                UpdateRotation(mEntity, targetPos, dt, turnSpeed);

                if (mWalkMode != ENUM_WALK_MODE_STOP)
                    ModalityChange(MODALITY_WALK);
            }
        }
    }

    void ArMoveProperty::Stop(bool bPassive /*= false*/)
    {
        mWalkMode = ENUM_WALK_MODE_STOP;
        ModalityChange(MODALITY_STAND);
    }

    void ArMoveProperty::TurnTo(const DiK2Pos& pos)
    {
    }

    void ArMoveProperty::MoveTo(const DiK2Pos& source, const DiK2Pos& target,
        float fRange /*= 0.0f*/)
    {
        //DI_DEBUG("MOVE from <%g,%g> to <%g,%g>", source.x, source.z, target.x, target.z);

        //DiTimer timer;
        auto& pathFinder = ArGameApp::Get()->GetWorld()->GetTerrain()->GetPathFinder();
        bool found = pathFinder.FindPath(&source, &target, mPosNode + 1, mNumNode, HeavyPathFinder::BLOCK_LEVEL_WALK);
        
        if (found)
        {
            mPosNode[0] = source;
            mNumNode++;
            mTargetPosition = target;
            mWalkMode = ENUM_WALK_MODE_WALK;
            mTargetDirection = INVALID_INT_VALUE;
            mNumCurTarget = 0;
            mDistance = 0;

            if (mNumNode > 0)
            {
                mPosNode[mNumNode - 1] = target;

                if (fRange > 0.001f)
                {
                    int nEndNodeIndex = INVALID_INT_VALUE;
                    DiK2Pos endTruePos(INVALID_FLOAT_VALUE, INVALID_FLOAT_VALUE);
                    for (int i = mNumNode - 2; i >= 0; i--)
                    {
                        if (mPosNode[i].Distance(target) >= fRange ||
                            !pathFinder.IsReachable(mPosNode[i], HeavyPathFinder::BLOCK_LEVEL_WALK))
                        {
                            nEndNodeIndex = i + 1;
                            endTruePos = GetIntersectionLineRound(mPosNode[i], mPosNode[i + 1], target, fRange);
                            break;
                        }
                    }
                    DI_ASSERT(nEndNodeIndex != INVALID_INT_VALUE);
                    DI_ASSERT(endTruePos.x != INVALID_FLOAT_VALUE);
                    DI_ASSERT(endTruePos.z != INVALID_FLOAT_VALUE);
                    mNumNode = nEndNodeIndex + 1;
                    mPosNode[mNumNode - 1] = endTruePos;
                }
            }
        }
        else
        {
            //DI_DEBUG("No available path");
        }
        //double loadingTime = timer.GetElapse();
        //DI_LOG("Pathfinding time: %f", loadingTime);
    }

    DiK2Pos ArMoveProperty::GetIntersectionLineRound(const DiK2Pos& linePos1, 
        const DiK2Pos& linePos2, const DiK2Pos& roundCenter, float fRadius)
    {
        DI_ASSERT(linePos1.Distance(roundCenter) >= fRadius);
        DI_ASSERT(linePos2.Distance(roundCenter) <= fRadius);

        DiVec3 vLinePos1(linePos1.x, 0.0f, linePos1.z);
        DiVec3 vLinePos2(linePos2.x, 0.0f, linePos2.z);
        DiVec3 vRoundCenter(roundCenter.x, 0.0f, roundCenter.z);
        vLinePos1 -= vRoundCenter;
        vLinePos2 -= vRoundCenter;
        DiVec3 vRotation1 = vLinePos1 - vLinePos2;
        vRotation1.normalise();
        DiVec3 vRotation2(1.0f, 0.0f, 0.0f);
        vRotation2.normalise();
        DiQuat quaternion = vRotation1.getRotationTo(vRotation2);
        vLinePos1 = quaternion*vLinePos1;
        vLinePos2 = quaternion*vLinePos2;
        float x = fRadius*fRadius - vLinePos1.z*vLinePos1.z;
        x = (float)sqrt(x);
        if (vLinePos1.x < vLinePos2.x)
            x = -x;
        DiVec3 vFinalPos(x, 0.0f, vLinePos1.z);
        DiQuat quaternion2 = vRotation2.getRotationTo(vRotation1);
        vFinalPos = quaternion2*vFinalPos;
        vFinalPos += vRoundCenter;
        DiK2Pos intersection(vFinalPos.x, vFinalPos.z);
        return intersection;
    }

    void ArMoveProperty::ModalityChange(ArModalityType mod)
    {
        if (mod != mModality)
        {
            mModality = mod;
            auto entity = GetEntity<ArDynEntity>();
            entity->GetMotionProperty()->ModalityChange(mod);
        }
    }

    void ArMoveProperty::StopAt(const DiK2Pos& pos)
    {

    }

    void ArMoveProperty::UpdateRotation(ArGameEntity* srcEntity, const DiK2Pos& targetPos, float dt, int turnRate)
    {
        DiK2RenderObject* renderObj = srcEntity->GetRenderObj();
        auto newPos = renderObj->GetPosition();

        DiVec3 direction = DiVec3(targetPos.x, 0, targetPos.z) - DiVec3(newPos.x, 0, newPos.z);
        direction.y = 0;
        direction.normalise();
        float yawToGoal;
        DiVec3 srcVec = renderObj->GetRotQuat() * DiVec3::UNIT_Z;

        if ((1.0f + srcVec.dotProduct(direction)) < 0.0001f)
            yawToGoal = 180;
        else
        {
            DiQuat toGoal = renderObj->GetRotQuat().zAxis().getRotationTo(direction);
            yawToGoal = toGoal.getYaw().valueDegrees();
        }

        float yawAtSpeed = yawToGoal / DiMath::Abs(yawToGoal) * dt * ((float)turnRate);
        if (yawToGoal < 0)
            yawToGoal = std::min<float>(0, std::max<float>(yawToGoal, yawAtSpeed));
        else if (yawToGoal > 0)
            yawToGoal = std::max<float>(0, std::min<float>(yawToGoal, yawAtSpeed));

        DiQuat actorOrientation = renderObj->GetRotQuat();
        actorOrientation = DiQuat(DiDegree(yawToGoal), DiVec3::UNIT_Y) * actorOrientation;

        float rotrad = actorOrientation.getYaw().valueRadians();
        renderObj->SetRotation(rotrad);
    }
}