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

#ifndef ArenaGameApp_h__
#define ArenaGameApp_h__

#include "ArenaPrerequisites.h"
#include "OIS.h"
#include "ArenaGame.h"

namespace Demi
{
    class ArGameApp : DiBase
    {
    public:

        ArGameApp();

        ~ArGameApp();

    public:

        static ArGameApp* Get() { return sApp; }

        static ArGameApp* sApp;

    public:

        void                Update();

        void                Close();

        bool                IsOpen();

        void                Open(int argc, char *argv[]);

        void                CloseEngine();

        ArInput*            GetInputManager() { return mInputMgr; }

        void                Run();

        void                OpenImpl();

        ArGame*             GetGame(){ return mGame; }

        DiK2World*          GetWorld() { return mGame->GetWorld(); }

        ArLevel*            GetLevel() { return mGame->GetLevel(); }

        ArEntityManager*    GetEntityManager(){ return mGame->GetEntityManager(); }

        void                QuitApp() { mQuit = true; }

    private:

        DiAssetManager*     mAssetManager;

        ArInput*            mInputMgr;

        ArGame*             mGame;

        bool                mQuit;
    };
}

#endif
