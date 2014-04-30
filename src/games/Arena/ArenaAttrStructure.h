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

#include "ArenaPrerequisites.h"

#include <memory.h>

namespace Demi
{
    struct ArNPCAttributes
    {
        ArNPCAttributes()
        {
            memset(this, 0, sizeof(*this));
            OBjID = INVALID_OBJ_ID;
        }

        ArObjID     OBjID;
        int         HP;
        int         MaxHP;
        int         Direction;
        float       MoveSpeed;
        int         TurnRate;   
    };

    struct ArHeroAttributes
    {
        ArHeroAttributes()
        {
            memset(this, 0, sizeof(*this));
            OBjID = INVALID_OBJ_ID;
        }

        ArObjID     OBjID;
        int         HP;
        int         MaxHP;
        int         Direction;
        float       MoveSpeed;
        int         TurnRate;
    };
}

#endif
