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
#include "DemoPch.h"
#include "CameraHelper.h"
#include "Camera.h"
#include "GfxDriver.h"
#include "RenderWindow.h"
#include "GfxDriver.h"

namespace Demi
{
    DiCameraHelper::DiCameraHelper( Demi::DiCamera* cam ) : mCamera(0)
        , mOrbiting(false)
        , mZooming(false)
        , mTopSpeed(150)
        , mVelocity(DiVec3::ZERO)
        , mTarget(DiVec3::ZERO)
        , mGoingForward(false)
        , mGoingBack(false)
        , mGoingLeft(false)
        , mGoingRight(false)
        , mGoingUp(false)
        , mGoingDown(false)
        , mFastMove(false)
        , mMoving(false)
        , mEnabled(true)
        , mMousePos(-1,-1)
    {
        SetCamera(cam);
        SetStyle(CS_ORBIT);    
    }

    void DiCameraHelper::SetYawPitchDist( DiRadian yaw, DiRadian pitch, float dist )
    {
        mCamera->SetPosition(mTarget);
        mCamera->SetOrientation(DiQuat::IDENTITY);
        mCamera->Yaw(yaw);
        mCamera->Pitch(-pitch);
        mCamera->MoveRelative(DiVec3(0, 0, dist));
    }

    void DiCameraHelper::SetStyle( DiCameraStyle style )
    {
        if (mStyle != CS_ORBIT && style == CS_ORBIT)
        {
            mCamera->SetFixedYawAxis(true);
            ManualStop();
            SetYawPitchDist(DiDegree(0), DiDegree(15), 150);
        }
        else if (mStyle != CS_FREELOOK && style == CS_FREELOOK)
        {
            mCamera->SetFixedYawAxis(true);
        }
        else if (mStyle != CS_MANUAL && style == CS_MANUAL)
        {
            ManualStop();
        }

        mStyle = style;
    }

    void DiCameraHelper::ManualStop()
    {
        if (mStyle == CS_FREELOOK)
        {
            mGoingForward = false;
            mGoingBack = false;
            mGoingLeft = false;
            mGoingRight = false;
            mGoingUp = false;
            mGoingDown = false;
            mVelocity = DiVec3::ZERO;
        }
    }

    bool DiCameraHelper::Update( float elapsed )
    {
        if (!mEnabled)
            return false;

        if (mStyle == CS_FREELOOK)
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
                mVelocity += accel * topSpeed * elapsed * 10;
            }
            else 
            {
                mVelocity -= mVelocity * elapsed * 10;
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
                mCamera->Move(mVelocity * elapsed);
            }
        }
        else if (mStyle == CS_SMOOTH)
        {
            if (Driver && mMousePos.x >= 0 && mMousePos.y >= 0)
            {
                DiRenderWindow* window = Driver->GetMainRenderWindow();
                int x = (mMousePos.x - window->GetWidth() / 2);
                int y = (mMousePos.y - window->GetHeight() / 2);
                DiVec3 pos = mCamera->GetPosition();
                pos.x += (x - pos.x) * 0.01f;
                pos.y += (-y - pos.y) * 0.01f;
                mCamera->SetPosition(pos);
                mCamera->LookAt(0, 0, 0);
            }
        }

        return true;
    }

    void DiCameraHelper::SetCamera( Demi::DiCamera* cam )
    {
        mCamera = cam;
    }

    Demi::DiCamera* DiCameraHelper::GetCamera()
    {
        return mCamera;
    }

    void DiCameraHelper::SetTopSpeed( float topSpeed )
    {
        mTopSpeed = topSpeed;
    }

    float DiCameraHelper::GetTopSpeed()
    {
        return mTopSpeed;
    }

    DiCameraStyle DiCameraHelper::GetStyle()
    {
        return mStyle;
    }
    
#if DEMI_PLATFORM == DEMI_PLATFORM_IOS
    void DiCameraHelper::OnMouseMove(const OIS::MultiTouchState& evt)
#else
    void DiCameraHelper::OnMouseMove(const OIS::MouseState& evt)
#endif
    {
        if (!mEnabled)
            return;
        
        mMousePos.x = evt.X.abs;
        mMousePos.y = evt.Y.abs;
        
        if (mStyle == CS_ORBIT)
        {
            float dist = (mCamera->GetPosition() - mTarget).length();
            
            if (mOrbiting)
            {
                mCamera->SetPosition(mTarget);
                mCamera->Yaw(DiDegree(-evt.X.rel * 0.25f));
                mCamera->Pitch(DiDegree(-evt.Y.rel * 0.25f));
                mCamera->MoveRelative(DiVec3(0, 0, dist));
            }
            else if (mZooming)
            {
                mCamera->MoveRelative(DiVec3(0, 0, evt.Y.rel * 0.004f * dist));
            }
            else if (evt.Z.rel != 0)
            {
                mCamera->MoveRelative(DiVec3(0, 0, -evt.Z.rel * 0.0008f * dist));
            }
            else if (mMoving)
            {
                DiVec3 vec = DiVec3((float)-evt.X.rel, (float)evt.Y.rel, 0.0f);
                mCamera->MoveRelative(vec);
                DiVec3 trans = mCamera->GetOrientation() * vec;
                mTarget += trans;
            }
        }
        else if (mStyle == CS_FREELOOK)
        {
            mCamera->Yaw(DiDegree(-evt.X.rel * 0.15f));
            mCamera->Pitch(DiDegree(-evt.Y.rel * 0.15f));
        }
    }
    
#if DEMI_PLATFORM == DEMI_PLATFORM_IOS
    void DiCameraHelper::OnMouseDown(const OIS::MultiTouchState& evt)
    {
        if (mStyle == CS_ORBIT)
        {
            mOrbiting = true;
        }
    }
    
    void DiCameraHelper::OnMouseUp(const OIS::MultiTouchState& evt)
    {
        if (mStyle == CS_ORBIT)
        {
            mOrbiting = false;
        }
    }
#else

    void DiCameraHelper::OnMouseDown(const OIS::MouseState& evt, OIS::MouseButtonID id)
    {
        if (!mEnabled)
            return;

        if (mStyle == CS_ORBIT)
        {
            if (id == OIS::MB_Left) 
                mOrbiting = true;
            else if (id == OIS::MB_Right)
                mZooming = true;
            else if (id == OIS::MB_Middle) 
                mMoving = true;
        }
    }

    void DiCameraHelper::OnMouseUp(const OIS::MouseState& evt, OIS::MouseButtonID id)
    {
        if (!mEnabled)
            return;

        if (mStyle == CS_ORBIT)
        {
            if (id == OIS::MB_Left)
                mOrbiting = false;
            else if (id == OIS::MB_Right)
                mZooming = false;
            else if (id == OIS::MB_Middle)
                mMoving = false;
        }
    }
#endif

    void DiCameraHelper::OnKeyDown(const OIS::KeyEvent& evt)
    {
        if (!mEnabled)
            return;

        if (mStyle == CS_FREELOOK)
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
    }

    void DiCameraHelper::OnKeyUp(const OIS::KeyEvent& evt)
    {
        if (!mEnabled)
            return;

        if (mStyle == CS_FREELOOK)
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

    void DiCameraHelper::ResetCamera()
    {
        mOrbiting = false;
        mZooming = false;
        mTopSpeed = 150;
        mVelocity = DiVec3::ZERO;
        mTarget = DiVec3::ZERO;
        mGoingForward = false;
        mGoingBack = false;
        mGoingLeft = false;
        mGoingRight = false;
        mGoingUp = false;
        mGoingDown = false;
        mFastMove = false;
        mMoving = false;
        
        mCamera->SetPosition(DiVec3::ZERO);
        mCamera->SetOrientation(DiQuat::IDENTITY);
        SetStyle(CS_FREELOOK);
    }

    void DiCameraHelper::Enable( bool enable )
    {
        mEnabled = enable;
    }
}
