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

#ifndef DiK2StateDefines_h__
#define DiK2StateDefines_h__

#include "K2Prerequisites.h"

namespace Demi
{
    /** States for characters (Hero & NPC)
    */
    enum K2CharState
    {
        STATE_STAND,
        STATE_WALK,
        STATE_ATTACK    // physical attack

    };

    typedef uint16 K2StateID;

    /** state definition
     */
    struct K2StateInfo
    {
        K2CharState stateType;
        
        K2StateID   stateID;

        // if lifeCycle < 0 means infinite time
        double      lifeCycle;  // time in s

        double      life;       // time in s

        bool        active;

        double      timeStamp;
    };
}

#endif
