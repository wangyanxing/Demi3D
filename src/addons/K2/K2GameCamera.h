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

#ifndef DiK2GameCamera_h__
#define DiK2GameCamera_h__

#include "K2Prerequisites.h"
#include "K2Input.h"

namespace Demi
{
    /** Fixed camera, base class
     */
    class DEMI_K2_API DiK2GameCamera : public DiBase
    {
    public:

        DiK2GameCamera(DiCamera* camera);

        virtual ~DiK2GameCamera();

        enum Style
        {
            Fixed_Style,
            Free_Style,
            Char_Style
        };

    public:

        virtual void    Update(float dt){}

        DiCamera*       GetCamera() { return mCamera; }

        virtual void    OnKeyInput(const K2KeyEvent& event) {}

        virtual void    OnMouseInput(const K2MouseEvent& event) {}

        Style           GetStyle() { return mStyle; }

    protected:

        DiCamera*       mCamera;

        float           mHeight;

        Style           mStyle;
    };

    /** Free camera
     */
    class DEMI_K2_API DiK2FreeCamera : public DiK2GameCamera
    {
    public:

        DiK2FreeCamera(DiCamera* camera);

        virtual ~DiK2FreeCamera();

    public:

        virtual void    OnMouseInput(const K2MouseEvent& event);

        virtual void    OnKeyInput(const K2KeyEvent& event);

        virtual void    Update(float dt);

    protected:

        bool            mGoingForward;

        bool            mGoingBack;

        bool            mGoingLeft;

        bool            mGoingRight;

        bool            mGoingUp;

        bool            mGoingDown;

        bool            mFastMove;

        bool            mMoving;

        float           mTopSpeed;

        DiVec3          mVelocity;
    };
}

#endif
