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

#ifndef ArenaStateDefines_h__
#define ArenaStateDefines_h__

#include "ArenaPrerequisites.h"

namespace Demi
{
    /** States for characters (Hero & NPC)
    */
    enum ArCharState
    {
        STATE_STAND,
        STATE_WALK,
        STATE_ATTACK    // physical attack
    };

    typedef uint16 ArStateID;

    /** state definition
     */
    struct ArStateInfo
    {
        ArCharState stateType;
        
        ArStateID   stateID;

        // if lifeCycle < 0 means infinite time
        double      lifeCycle;  // time in s

        double      life;       // time in s

        bool        active;

        double      timeStamp;
    };
}

#endif
