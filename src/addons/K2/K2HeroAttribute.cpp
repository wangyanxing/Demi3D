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

#include "K2Pch.h"
#include "K2HeroAttribute.h"

namespace Demi
{
    DiK2HeroAttr::DiK2HeroAttr()
    {
    }

    DiK2HeroAttr::~DiK2HeroAttr()
    {
    }

    const K2ObjID DiK2HeroAttr::GetID() const
    {
        return mHeroAttrs.OBjID;
    }

    const int DiK2HeroAttr::GetHP() const
    {
        return mHeroAttrs.HP;
    }

    const int DiK2HeroAttr::GetMaxHP() const
    {
        return mHeroAttrs.MaxHP;
    }

    const int DiK2HeroAttr::GetDirection() const
    {
        return mHeroAttrs.Direction;
    }

    const float DiK2HeroAttr::GetMoveSpeed() const
    {
        return mHeroAttrs.MoveSpeed;
    }

}