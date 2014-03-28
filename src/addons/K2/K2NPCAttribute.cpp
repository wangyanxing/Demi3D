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
#include "K2NPCAttribute.h"

namespace Demi
{
    DiK2NPCAttr::DiK2NPCAttr()
    {
    }

    DiK2NPCAttr::~DiK2NPCAttr()
    {
    }

    const K2ObjID DiK2NPCAttr::GetID() const
    {
        return mNPCAttrs.OBjID;

    }

    const int DiK2NPCAttr::GetHP() const
    {
        return mNPCAttrs.HP;
    }

    const int DiK2NPCAttr::GetMaxHP() const
    {
        return mNPCAttrs.MaxHP;
    }

    const int DiK2NPCAttr::GetDirection() const
    {
        return mNPCAttrs.Direction;
    }

    const float DiK2NPCAttr::GetMoveSpeed() const
    {
        return mNPCAttrs.MoveSpeed;
    }

}