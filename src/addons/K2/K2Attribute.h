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

#ifndef DiK2Attribute_h__
#define DiK2Attribute_h__

#include "K2Prerequisites.h"

namespace Demi
{
    /** Attribute interface for entities
     */
    class DEMI_K2_API DiK2Attribute
    {
    public:

        virtual const K2ObjID   GetID() const = 0;

        virtual const int       GetHP() const = 0;

        virtual const int       GetMaxHP() const = 0;

        virtual const int       GetDirection() const = 0;

        virtual const float     GetMoveSpeed() const = 0;

        virtual void            Update(float dt) {}
    };
}

#endif
