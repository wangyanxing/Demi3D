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

#ifndef ArenaAICommand_h__
#define ArenaAICommand_h__

#include "ArenaPrerequisites.h"

namespace Demi
{
    enum ArAIType
    {
        ENUM_AI_TYPE_EMPTY = 0,
        ENUM_AI_TYPE_MOVETOPOINT,
        ENUM_AI_TYPE_MOVETOTARGET,
        ENUM_AI_TYPE_USESKILL,
        ENUM_AI_TYPE_USESKILLWITHPOS,
        ENUM_AI_TYPE_TALKTO,
        ENUM_AI_TYPE_PICKPACKAGE,

        ENUM_AI_TYPE_COUNT,
    };

    class ArAICommand
    {
    public:
        ArAICommand(ArEntity* pEntity);

        virtual             ~ArAICommand(){}

    public:

        virtual void        Enter(){}
        
        virtual void        Leave(){}
        
        virtual void        Update(float dt) = 0;
        
        virtual void        Redo(){}

        virtual bool        IsEnd() const
        {
            return !mValid;
        }

        virtual void        EndCommand()
        {
            mValid = false;
        }

        virtual ArAIType    GetType()
        {
            return mType;
        }

    protected:
        
        ArAIType            mType;
        
        ArNPCEntity*        mEntity;
        
        bool                mValid;
    };
}

#endif
