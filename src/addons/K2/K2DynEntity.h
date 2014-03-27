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

#ifndef DiK2DynEntity_h__
#define DiK2DynEntity_h__

#include "K2Prerequisites.h"
#include "K2GameEntity.h"
#include "K2MoveProperty.h"

namespace Demi
{
    /** Dynamic entity
     */
    class DEMI_K2_API DiK2DynEntity : public DiK2GameEntity
    {
        DEF_DECLARE_PRIORITY(MoveProperty);

    public:

        DiK2DynEntity();

        virtual         ~DiK2DynEntity();

    public:

        virtual void    InitComponents();

    private:
    };
}

#endif
