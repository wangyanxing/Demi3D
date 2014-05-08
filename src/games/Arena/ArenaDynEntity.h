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

#ifndef ArenaDynEntity_h__
#define ArenaDynEntity_h__

#include "ArenaPrerequisites.h"
#include "ArenaGameEntity.h"
#include "ArenaMoveProperty.h"
#include "ArenaStateProperty.h"
#include "ArenaMotionProperty.h"

namespace Demi
{
    /** Dynamic entity
     */
    class ArDynEntity : public ArGameEntity
    {
        DEF_DECLARE_PRIORITY(MotionProperty)
        DEF_DECLARE_PRIORITY(StateProperty)
        DEF_DECLARE_PRIORITY(MoveProperty)


    public:

        ArDynEntity();

        virtual         ~ArDynEntity();

    public:

        virtual void    InitComponents() override;

    private:
    };
}

#endif
