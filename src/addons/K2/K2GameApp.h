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

#ifndef DiK2GameApp_h__
#define DiK2GameApp_h__

#include "K2Prerequisites.h"

#include "OIS.h"
#include "K2Game.h"

namespace Demi
{
    class DEMI_K2_API DiK2GameApp : DiBase
    {
    public:

        DiK2GameApp();

        ~DiK2GameApp();

    public:

        static DiK2GameApp* Get() { return sApp; }

        static DiK2GameApp* sApp;

    public:

        void                Update();

        void                Close();

        bool                IsOpen();

        void                Open();

        void                CloseEngine();

        DiK2Input*          GetInputManager() { return mInputMgr; }

        void                OnKeyPressed(const OIS::KeyEvent &arg);

        void                OnKeyReleased(const OIS::KeyEvent &arg);

        void                Run();

        void                OpenImpl();

        DiK2Game*           GetGame(){ return mGame; }

        DiK2World*          GetWorld() { return mGame->GetWorld(); }

        DiK2EntityManager*  GetEntityManager(){ return mGame->GetEntityManager(); }

    private:

        DiAssetManager*     mAssetManager;

        DiK2Input*          mInputMgr;

        DiK2Game*           mGame;

        bool                mQuit;
    };
}

#endif
