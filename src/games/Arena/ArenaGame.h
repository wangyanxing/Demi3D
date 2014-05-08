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

#ifndef ArenaGame_h__
#define ArenaGame_h__

#include "ArenaPrerequisites.h"
#include "ArenaInput.h"
#include "ArenaGameCamera.h"

namespace Demi
{
    class ArGame : public DiBase
    {
    public:

        ArGame();

        ~ArGame();

    public:

        void                LoadLevel(const DiString& path);

        DiK2World*          GetWorld();

        ArLevel*            GetLevel() { return mLevel; }

        ArEntityManager*    GetEntityManager() { return mEntityMgr; }

        void                SetHero(const DiString& configFile);

        void                Update();

        void                OnKeyInput(const K2KeyEvent& event);

        void                OnMouseInput(const K2MouseEvent& event);

        void                SetCamera(ArGameCamera::Style style);

        ArGameCamera*       GetCamera() { return mCamera; }

    private:

        void                Init();

    private:

        typedef DiMap<ArGameCamera::Style, ArGameCamera*> Cameras;

        Cameras             mCameraPool;

        ArGameCamera*       mCamera;

        ArLevel*            mLevel;

        ArHeroEntityPtr     mHero;

        ArEntityManager*    mEntityMgr;
    };
}

#endif
