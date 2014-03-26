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
#include "K2Property.h"
#include "K2GameEntity.h"

namespace Demi
{
    void DiK2Property::setEntity(DiK2Entity* entity)
    {
        DI_ASSERT(entity);
        mEntity = static_cast<DiK2GameEntity*>(entity);
        onEntityReady();
    }
}