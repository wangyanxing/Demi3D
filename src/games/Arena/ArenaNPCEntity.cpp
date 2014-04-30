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
#include "ArenaNPCEntity.h"
#include "ArenaNPCAttribute.h"

namespace Demi
{
    ArNPCEntity::ArNPCEntity()
    {
    }

    ArNPCEntity::~ArNPCEntity()
    {
    }

    void ArNPCEntity::InitComponents()
    {
        ArDynEntity::InitComponents();
    }

    void ArNPCEntity::InitAttribute()
    {
        SetAttribute<ArNPCAttr>();
    }
}