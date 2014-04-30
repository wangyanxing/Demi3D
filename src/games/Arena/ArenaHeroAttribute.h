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

#ifndef ArenaHeroAttribute_h__
#define ArenaHeroAttribute_h__

#include "ArenaPrerequisites.h"
#include "ArenaAttribute.h"
#include "ArenaAttrStructure.h"

namespace Demi
{
    class DiK2HeroAttr : public ArAttribute
    {
    public:

        DiK2HeroAttr();

        virtual ~DiK2HeroAttr();

    public:

        virtual ArObjID   GetID() const;

        virtual int       GetHP() const;

        virtual int       GetMaxHP() const;

        virtual int       GetDirection() const;

        virtual float     GetMoveSpeed() const;

        virtual void      Update(float dt) {}

    protected:

        ArHeroAttributes  mHeroAttrs;
    };
}

#endif
