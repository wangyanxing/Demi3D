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

#ifndef ArenaHeroEntity_h__
#define ArenaHeroEntity_h__

#include "ArenaPrerequisites.h"
#include "ArenaNPCEntity.h"
#include "ArenaInput.h"

namespace Demi
{
    class ArHeroEntity : public ArNPCEntity
    {
    public:

        ArHeroEntity();

        ~ArHeroEntity();

    public:

        void OnKeyInput(const K2KeyEvent& event);

        void OnMouseInput(const K2MouseEvent& event);

        void InitAttribute();

    private:
    };
}

#endif
