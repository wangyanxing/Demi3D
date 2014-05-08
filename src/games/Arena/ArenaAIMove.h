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

#ifndef ArenaAIMove_h__
#define ArenaAIMove_h__

#include "ArenaPrerequisites.h"
#include "ArenaAICommand.h"

namespace Demi
{
    class ArAIMoveCommand : public ArAICommand
    {
    public:
        ArAIMoveCommand(ArEntity* pEntity, const DiK2Pos& pos, float range = 0);

        ~ArAIMoveCommand();

    public:

        void        Enter();

        void        Leave();

        void        Update(float dt);

        void        Redo();

        void        Move();

    protected:

        void	    Stop();

    protected:
        
        DiK2Pos     mPos;

        float       mRange;
    };
}

#endif
