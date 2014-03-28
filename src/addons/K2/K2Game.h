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

#ifndef DiK2Game_h__
#define DiK2Game_h__

#include "K2Prerequisites.h"
#include "K2Input.h"
#include "K2GameCamera.h"

namespace Demi
{
    class DEMI_K2_API DiK2Game : public DiBase
    {
    public:

        DiK2Game();

        ~DiK2Game();

    public:

        void                OpenWorld(const DiString& path);

        DiK2World*          GetWorld(){ return mWorld; }

        DiK2EntityManager*  GetEntityManager() { return mEntityMgr; }

        void                SetHero(const DiString& model);

        void                Update();

        void                OnKeyInput(const K2KeyEvent& event);

        void                OnMouseInput(const K2MouseEvent& event);

        void                SetCamera(DiK2GameCamera::Style style);

        DiK2GameCamera*     GetCamera() { return mCamera; }

    private:

        void                Init();

    private:

        typedef DiMap<DiK2GameCamera::Style, DiK2GameCamera*> Cameras;

        Cameras             mCameraPool;

        DiK2GameCamera*     mCamera;

        DiK2World*          mWorld;

        DiK2HeroEntityPtr   mHero;

        DiK2EntityManager*  mEntityMgr;
    };
}

#endif
