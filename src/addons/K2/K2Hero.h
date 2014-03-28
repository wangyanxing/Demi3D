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

#ifndef DiK2HeroEntity_h__
#define DiK2HeroEntity_h__

#include "K2Prerequisites.h"
#include "K2NPCEntity.h"
#include "K2Input.h"

namespace Demi
{
    class DEMI_K2_API DiK2HeroEntity : public DiK2NPCEntity
    {
    public:

        DiK2HeroEntity();

        ~DiK2HeroEntity();

    public:

        void        OnKeyInput(const K2KeyEvent& event);

        void        OnMouseInput(const K2MouseEvent& event);

        void        InitAttribute();

    private:
    };
}

#endif
