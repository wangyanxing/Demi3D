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

#include "ArenaPch.h"
#include "ArenaHeroAttribute.h"

namespace Demi
{
    DiK2HeroAttr::DiK2HeroAttr()
    {
    }

    DiK2HeroAttr::~DiK2HeroAttr()
    {
    }

    ArObjID DiK2HeroAttr::GetID() const
    {
        return mHeroAttrs.OBjID;
    }

    int DiK2HeroAttr::GetHP() const
    {
        return mHeroAttrs.HP;
    }

    int DiK2HeroAttr::GetMaxHP() const
    {
        return mHeroAttrs.MaxHP;
    }

    int DiK2HeroAttr::GetDirection() const
    {
        return mHeroAttrs.Direction;
    }

    float DiK2HeroAttr::GetMoveSpeed() const
    {
        return mHeroAttrs.MoveSpeed;
    }

}