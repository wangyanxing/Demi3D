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
#include "OIS.h"

namespace Demi
{
    class DEMI_K2_API DiK2Game : public DiBase
    {
    public:

        DiK2Game();

        ~DiK2Game();

    public:

        void        OpenWorld(const DiString& path);

        DiK2World*  GetWorld(){ return mWorld; }

        void        SetHero(const DiString& model);

        void        Update();

        void        KeyPressed(const OIS::KeyEvent &arg);

        void        KeyReleased(const OIS::KeyEvent &arg);

        void        MouseMoved(const OIS::MouseEvent& evt);

        void        MousePressed(const OIS::MouseEvent& evt, OIS::MouseButtonID id);

        void        MouseReleased(const OIS::MouseEvent& evt, OIS::MouseButtonID id);

    private:

        void        Init();

    private:

        DiK2World*  mWorld;

        DiK2HeroEntity*   mHero;

        DiK2GameCamera* mCamera;
    };
}

#endif
