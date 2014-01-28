
/********************************************************************
    File:       CameraManager.h
    Creator:    demiwangya
*********************************************************************/

#pragma once

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

        virtual                    ~DiCameraHelper() {}

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

        void                    OnMouseMove(const OIS::MouseEvent& evt);

        void                    OnMouseDown(const OIS::MouseEvent& evt, OIS::MouseButtonID id);

        void                    OnMouseUp(const OIS::MouseEvent& evt, OIS::MouseButtonID id);

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