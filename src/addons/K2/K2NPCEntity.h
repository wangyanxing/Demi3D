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

#ifndef DiK2NPCEntity_h__
#define DiK2NPCEntity_h__

#include "K2Prerequisites.h"
#include "K2DynEntity.h"

namespace Demi
{
    /** NPC entity
     */
    class DEMI_K2_API DiK2NPCEntity : public DiK2DynEntity
    {
    public:

        DiK2NPCEntity();

        virtual         ~DiK2NPCEntity();

    public:

        virtual void    InitComponents();

    private:
    };
}

#endif
