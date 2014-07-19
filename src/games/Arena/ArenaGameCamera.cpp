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
#include "ArenaGameCamera.h"
#include "ArenaHero.h"
#include "K2RenderObjects.h"

#include "CullNode.h"

namespace Demi
{
    ArGameCamera::ArGameCamera(DiCamera* camera)
        : mCamera(camera)
        , mHeight(900)
        , mStyle(STYLE_FIXED)
        , mTarget(nullptr)
    {
    }

    ArGameCamera::~ArGameCamera()
    {
    }

    DiK2FreeCamera::DiK2FreeCamera(DiCamera* camera)
        : ArGameCamera(camera)
        , mGoingForward(false)
        , mGoingBack(false)
        , mGoingLeft(false)
        , mGoingRight(false)
        , mGoingUp(false)
        , mGoingDown(false)
        , mFastMove(false)
        , mMoving(false)
        , mTopSpeed(150)
        , mVelocity(DiVec3::ZERO)
    {
        mStyle = STYLE_FREE;
    }

    DiK2FreeCamera::~DiK2FreeCamera()
    {

    }

    void DiK2FreeCamera::OnKeyInput(const K2KeyEvent& evt)
    {
        if (evt.type == K2KeyEvent::KEY_PRESS)
        {
            if (evt.key == OIS::KC_W || evt.key == OIS::KC_UP)
                mGoingForward = true;
            else if (evt.key == OIS::KC_S || evt.key == OIS::KC_DOWN)
                mGoingBack = true;
            else if (evt.key == OIS::KC_A || evt.key == OIS::KC_LEFT)
                mGoingLeft = true;
            else if (evt.key == OIS::KC_D || evt.key == OIS::KC_RIGHT)
                mGoingRight = true;
            else if (evt.key == OIS::KC_PGUP)
                mGoingUp = true;
            else if (evt.key == OIS::KC_PGDOWN)
                mGoingDown = true;
            else if (evt.key == OIS::KC_LSHIFT)
                mFastMove = true;
        }
        else if (evt.type == K2KeyEvent::KEY_RELEASE)
        {
            if (evt.key == OIS::KC_W || evt.key == OIS::KC_UP)
                mGoingForward = false;
            else if (evt.key == OIS::KC_S || evt.key == OIS::KC_DOWN)
                mGoingBack = false;
            else if (evt.key == OIS::KC_A || evt.key == OIS::KC_LEFT)
                mGoingLeft = false;
            else if (evt.key == OIS::KC_D || evt.key == OIS::KC_RIGHT)
                mGoingRight = false;
            else if (evt.key == OIS::KC_PGUP)
                mGoingUp = false;
            else if (evt.key == OIS::KC_PGDOWN)
                mGoingDown = false;
            else if (evt.key == OIS::KC_LSHIFT)
                mFastMove = false;
        }
    }

    void DiK2FreeCamera::OnMouseInput(const K2MouseEvent& event)
    {
        if (event.type == K2MouseEvent::MOUSE_MOVE)
        {
            mCamera->Yaw(DiDegree(-event.state.X.rel * 0.15f));
            mCamera->Pitch(DiDegree(-event.state.Y.rel * 0.15f));
        }
        else if (event.type == K2MouseEvent::MOUSE_PRESS)
        {
        }
        else if (event.type == K2MouseEvent::MOUSE_RELEASE)
        {
        }
    }

    void DiK2FreeCamera::Update(float dt)
    {
        DiVec3 accel = DiVec3::ZERO;
        if (mGoingForward)
            accel += mCamera->GetDirection();
        if (mGoingBack)
            accel -= mCamera->GetDirection();
        if (mGoingRight)
            accel += mCamera->GetRight();
        if (mGoingLeft)
            accel -= mCamera->GetRight();
        if (mGoingUp)
            accel += mCamera->GetUp();
        if (mGoingDown)
            accel -= mCamera->GetUp();

        float topSpeed = mFastMove ? mTopSpeed * 20 : mTopSpeed;
        if (accel.squaredLength() != 0)
        {
            accel.normalise();
            mVelocity += accel * topSpeed * dt * 10;
        }
        else
            mVelocity -= mVelocity * dt * 10;

        float tooSmall = std::numeric_limits<float>::epsilon();

        if (mVelocity.squaredLength() > topSpeed * topSpeed)
        {
            mVelocity.normalise();
            mVelocity *= topSpeed;
        }
        else if (mVelocity.squaredLength() < tooSmall * tooSmall)
            mVelocity = DiVec3::ZERO;

        if (mVelocity != DiVec3::ZERO)
            mCamera->Move(mVelocity * dt);
    }


    DiK2CharCamera::DiK2CharCamera(DiCamera* camera)
        : ArGameCamera(camera)
    {
    }

    DiK2CharCamera::~DiK2CharCamera()
    {
    }

    void DiK2CharCamera::OnMouseInput(const K2MouseEvent& event)
    {

    }

    void DiK2CharCamera::OnKeyInput(const K2KeyEvent& event)
    {

    }

    void DiK2CharCamera::Update(float dt)
    {
        if (!mTarget)
            return;

        DiVec3 pos = mTarget->GetWorldPosition();
        pos.y += mHeight;
        pos.z += mHeight;
        mCamera->SetPosition(pos);
        mCamera->LookAt(mTarget->GetWorldPosition());
#if 0
        DiQuat q;
        q.FromAngleAxis(DiRadian(-DiMath::HALF_PI), DiVec3::UNIT_X);
        mCamera->SetOrientation(q);
#endif
    }
}