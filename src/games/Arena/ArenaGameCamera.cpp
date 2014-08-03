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
    {
        mStyle = STYLE_FREE;
    }

    DiK2FreeCamera::~DiK2FreeCamera()
    {

    }

    void DiK2FreeCamera::OnKeyInput(const K2KeyEvent& evt)
    {
    }

    void DiK2FreeCamera::OnMouseInput(const K2MouseEvent& event)
    {
        if (event.type == K2MouseEvent::MOUSE_MOVE && mMoving)
        {
            DiVec3 accel = DiVec3::ZERO;
            float speed = 0.1f;

            if(event.state.X.rel < 0)
            {
                accel += DiVec3(-event.state.X.rel * speed,0,0);
            }
            else if(event.state.X.rel > 0)
            {
                accel += DiVec3(-event.state.X.rel * speed,0,0);
            }
            
            if(event.state.Y.rel < 0)
            {
                accel += DiVec3(0,0,-event.state.Y.rel * speed);
            }
            else if(event.state.Y.rel > 0)
            {
                accel += DiVec3(0,0,-event.state.Y.rel * speed);
            }
    
            DiVec3 vel = accel * 30;
            
            mCamera->Move(vel);
        }
        else
        {
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