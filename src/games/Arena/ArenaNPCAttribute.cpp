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
#include "ArenaNPCAttribute.h"

namespace Demi
{
    ArNPCAttr::ArNPCAttr()
    {
    }

    ArNPCAttr::~ArNPCAttr()
    {
    }

    ArObjID ArNPCAttr::GetID() const
    {
        return mNPCAttrs.OBjID;

    }

    int ArNPCAttr::GetHP() const
    {
        return mNPCAttrs.HP;
    }

    int ArNPCAttr::GetMaxHP() const
    {
        return mNPCAttrs.MaxHP;
    }

    int ArNPCAttr::GetDirection() const
    {
        return mNPCAttrs.Direction;
    }

    float ArNPCAttr::GetMoveSpeed() const
    {
        return mNPCAttrs.MoveSpeed;
    }

}