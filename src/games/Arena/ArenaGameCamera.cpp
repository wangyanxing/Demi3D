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
        , mHeight(800)
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
            else if (evt.key == OIS::KC_Q)
                mGoingUp = true;
            else if (evt.key == OIS::KC_E)
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
            else if (evt.key == OIS::KC_Q)
                mGoingUp = false;
            else if (evt.key == OIS::KC_E)
                mGoingDown = false;
            else if (evt.key == OIS::KC_LSHIFT)
                mFastMove = false;
        }
    }

    void DiK2FreeCamera::OnMouseInput(const K2MouseEvent& event)
    {
        if (event.type == K2MouseEvent::MOUSE_MOVE && mMoving)
        {
            //mCamera->Yaw(DiDegree(-event.state.X.rel * 0.15f));
            //mCamera->Pitch(DiDegree(-event.state.Y.rel * 0.15f));
            
            DiVec3 accel = DiVec3::ZERO;

            if(event.state.X.rel < 0)
            {
                mGoingLeft = true;
                accel -= DiVec3::UNIT_X;
            }
            else if(event.state.X.rel > 0)
            {
                mGoingRight = true;
                accel += DiVec3::UNIT_X;
            }
            else
            {
                mGoingLeft = false;
                mGoingRight = false;
            }
            /*
            if(event.state.Y.rel < 0)
            {
                mGoingForward = true;
                accel -= DiVec3::UNIT_Z;
            }
            else if(event.state.Y.rel > 0)
            {
                mGoingBack = true;
                accel += DiVec3::UNIT_Z;
            }
            else
            {
                mGoingForward = false;
                mGoingBack = false;
            }*/
            
            DiVec3 vel = accel * 15;
            
            //mCamera->Move(vel);
        }
        else
        {
            mGoingForward = false;
            mGoingBack = false;
            mGoingLeft = false;
            mGoingRight = false;
         
            if (event.type == K2MouseEvent::MOUSE_PRESS)
            {
                mMoving = true;
            }
            else if (event.type == K2MouseEvent::MOUSE_RELEASE)
            {
                mMoving = false;
            }
        }
    }

    void DiK2FreeCamera::Update(float dt)
    {
        //return;
        if(!mMoving)
            return;
        
        DiVec3 accel = DiVec3::ZERO;
        if (mGoingForward)
            accel -= DiVec3::UNIT_Z;
        if (mGoingBack)
            accel += DiVec3::UNIT_Z;
        if (mGoingRight)
            accel += DiVec3::UNIT_X;
        if (mGoingLeft)
            accel -= DiVec3::UNIT_X;
        if (mGoingUp)
            accel += mCamera->GetDirection();
        if (mGoingDown)
            accel -= mCamera->GetDirection();

        float topSpeed = mFastMove ? mTopSpeed * 20 : mTopSpeed;
        if (accel.squaredLength() != 0)
        {
            accel.normalise();
            mVelocity += accel * topSpeed * dt * 10;
        }
        else
        {
            mVelocity -= mVelocity * dt * 10;
        }

        float tooSmall = std::numeric_limits<float>::epsilon();

        if (mVelocity.squaredLength() > topSpeed * topSpeed)
        {
            mVelocity.normalise();
            mVelocity *= topSpeed;
        }
        else if (mVelocity.squaredLength() < tooSmall * tooSmall)
        {
            mVelocity = DiVec3::ZERO;
        }

        if (mVelocity != DiVec3::ZERO)
        {
            mCamera->Move(mVelocity * dt);
        }
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
        {
            return;
        }

        DiVec3 pos = mTarget->GetWorldPosition();
        pos.y += mHeight;
        pos.z += mHeight;
        mCamera->SetPosition(pos);
        mCamera->LookAt(mTarget->GetWorldPosition());
    }
}