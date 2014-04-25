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

#ifndef DiCameraHelper_h__
#define DiCameraHelper_h__


#include "Camera.h"
#include "OIS.h"

namespace Demi
{
    enum DiCameraStyle
    {
        CS_FREELOOK,
        CS_ORBIT,
        CS_SMOOTH,
        CS_MANUAL
    };

    //////////////////////////////////////////////////////////////////////////

    class DEMI_DEMO_API DiCameraHelper : public DiBase
    {
    public:

        DiCameraHelper(Demi::DiCamera* cam);

        virtual                 ~DiCameraHelper() {}

    public:

        void                    SetCamera(DiCamera* cam);

        DiCamera*               GetCamera();

        void                    ResetCamera();

        void                    SetYawPitchDist(DiRadian yaw, DiRadian pitch, float dist);

        void                    SetTopSpeed(float topSpeed);

        float                   GetTopSpeed();

        void                    SetStyle(DiCameraStyle style);

        DiCameraStyle           GetStyle();

        void                    ManualStop();

        bool                    Update(float elapsed);
        
#if DEMI_PLATFORM == DEMI_PLATFORM_IOS
        void                    OnMouseMove(const OIS::MultiTouchEvent& evt);
        
        void                    OnMouseDown(const OIS::MultiTouchEvent& evt);
        
        void                    OnMouseUp(const OIS::MultiTouchEvent& evt);
#else
        void                    OnMouseMove(const OIS::MouseState& evt);

        void                    OnMouseDown(const OIS::MouseState& evt, OIS::MouseButtonID id);

        void                    OnMouseUp(const OIS::MouseState& evt, OIS::MouseButtonID id);
#endif

        void                    OnKeyDown(const OIS::KeyEvent& evt);

        void                    OnKeyUp(const OIS::KeyEvent& evt);

        bool                    IsOrbiting(){return mOrbiting;}

        bool                    IsMoving(){return mMoving;}

        bool                    IsZooming(){return mZooming;}

        void                    Enable(bool enable);

    protected:

        bool                    mEnabled;

        DiIntVec2               mMousePos;

        DiCamera*               mCamera;

        DiCameraStyle           mStyle;
        
        bool                    mOrbiting;
        
        bool                    mZooming;
        
        float                   mTopSpeed;
        
        DiVec3                  mVelocity;
        
        DiVec3                  mTarget;
        
        bool                    mGoingForward;
        
        bool                    mGoingBack;

        bool                    mGoingLeft;

        bool                    mGoingRight;

        bool                    mGoingUp;

        bool                    mGoingDown;

        bool                    mFastMove;
        
        bool                    mMoving;
    };
}

#endif
