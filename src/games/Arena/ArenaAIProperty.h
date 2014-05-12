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

#ifndef ArenaAIProperty_h__
#define ArenaAIProperty_h__

#include "ArenaPrerequisites.h"
#include "ArenaProperty.h"

namespace Demi
{
    /** AI property
     */
    class ArAIProperty : public ArProperty
    {
        DEFINE_PROPERTY_ID(PROPERTY_AI)

    public:

        ArAIProperty();

        ~ArAIProperty();

    public:

        void        StopAI(const DiK2Pos& pos);

        void        ClearAI();

        void        Update(float dt);

        void        CommandMoveTo(const DiK2Pos& pos, float range = 0.0f);

        void        CommandMoveTo(ArObjID targetID, float range = 0.0f);

        void        CommandFollowTo(ArObjID targetID, float range = 0.0f);

        void        CommandAttack(ArObjID targetID);

    protected:

        void        ClearAIList();

        void        PushCommand(ArAICommand* cmd);

        void        PopCommand();

        void        UpdateDummyAI(float dt);

    private:

        typedef DiVector<ArAICommand*> AICmdList;
        AICmdList   mAIList;

        double      mLastMoveTime{ 0.0 };
    };
}

#endif
