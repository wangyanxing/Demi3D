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

#ifndef ArenaAIFollowTarget_h__
#define ArenaAIFollowTarget_h__

#include "ArenaPrerequisites.h"
#include "ArenaAICommand.h"

namespace Demi
{
    class ArAIFollowCommand : public ArAICommand
    {
    public:
        ArAIFollowCommand(ArEntity* pEntity, ArObjID target, float range = 0);

        ~ArAIFollowCommand();

    public:

        void        Enter();

        void        Leave();

        void        Update(float dt);

        void        Redo();

        void        Move();

    protected:

        void	    Stop();

        bool        CheckDistance();

    protected:
        
        ArObjID     mTargetID{ INVALID_OBJ_ID };

        float       mRange{ 0.0f };

        DiK2Pos     mLastPos{ INVALID_FLOAT_VALUE, INVALID_FLOAT_VALUE };

        double      mElapse{ 0.0 };

        bool        mPassive{ false };

        ArNPCEntityPtr mTargetEntity;
    };
}

#endif
