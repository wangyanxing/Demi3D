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

#ifndef ArenaGameCamera_h__
#define ArenaGameCamera_h__

#include "ArenaPrerequisites.h"
#include "ArenaInput.h"

namespace Demi
{
    /** Fixed camera, base class
     */
    class ArGameCamera : public DiBase
    {
    public:

        ArGameCamera(DiCamera* camera);

        virtual ~ArGameCamera();

        enum Style
        {
            STYLE_FIXED,
            STYLE_FREE,
            STYLE_CHARACTER
        };

    public:

        virtual void        Update(float dt){}

        DiCamera*           GetCamera() { return mCamera; }

        virtual void        OnKeyInput(const K2KeyEvent& event) {}

        virtual void        OnMouseInput(const K2MouseEvent& event) {}

        Style               GetStyle() { return mStyle; }

        void                SetTarget(DiK2RenderObject* target){ mTarget = target; }

    protected:

        DiCamera*           mCamera;

        float               mHeight;

        Style               mStyle;

        DiK2RenderObject*   mTarget;
    };

    /** Free camera
     */
    class DiK2FreeCamera : public ArGameCamera
    {
    public:

        DiK2FreeCamera(DiCamera* camera);

        virtual ~DiK2FreeCamera();

    public:

        virtual void    OnMouseInput(const K2MouseEvent& event);

        virtual void    OnKeyInput(const K2KeyEvent& event);

        virtual void    Update(float dt);
        
    protected:
        
        bool mMoving{ false };
    };

    /** Character camera
     */
    class DiK2CharCamera : public ArGameCamera
    {
    public:

        DiK2CharCamera(DiCamera* camera);

        virtual ~DiK2CharCamera();

    public:

        virtual void    OnMouseInput(const K2MouseEvent& event);

        virtual void    OnKeyInput(const K2KeyEvent& event);

        virtual void    Update(float dt);
    };
}

#endif
