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

#ifndef DiK2AttrStructure_h__
#define DiK2AttrStructure_h__

#include "K2Prerequisites.h"
#include "K2GameDefines.h"

#include <memory.h>

namespace Demi
{
    struct K2NPCAttributes
    {
        K2NPCAttributes()
        {
            memset(this, 0, sizeof(*this));
            OBjID = INVALID_OBJ_ID;
        }

        K2ObjID     OBjID;
        int         HP;
        int         MaxHP;
        int         Direction;
        float       MoveSpeed;
        int         TurnRate;   
    };

    struct K2HeroAttributes
    {
        K2HeroAttributes()
        {
            memset(this, 0, sizeof(*this));
            OBjID = INVALID_OBJ_ID;
        }

        K2ObjID     OBjID;
        int         HP;
        int         MaxHP;
        int         Direction;
        float       MoveSpeed;
        int         TurnRate;
    };
}

#endif
