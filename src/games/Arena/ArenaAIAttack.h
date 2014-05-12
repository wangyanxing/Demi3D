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

#ifndef ArenaAIAttack_h__
#define ArenaAIAttack_h__

#include "ArenaPrerequisites.h"
#include "ArenaAICommand.h"

namespace Demi
{
    class ArAIAttackCommand : public ArAICommand
    {
    public:
        ArAIAttackCommand(ArEntity* pEntity, ArObjID target);

        ~ArAIAttackCommand();

    public:

        void        Enter();

        void        Leave();

        void        Update(float dt);

        void        Redo();

        void        PlayClip();

    protected:
        
        ArObjID     mTarget{ INVALID_OBJ_ID };

        float       mCoolDownTime{ 0.0f };

        int         mNumAttackAnims{ 0 };

        int         mLastAnimID{ -1 };

        float       mTimer{ 0.0f };
    };
}

#endif
