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

#ifndef BoxEmitterObject_h__
#define BoxEmitterObject_h__

#include "FxerPrerequisites.h"
#include "EmitterBaseObj.h"

namespace Demi 
{
    class DiBoxEmitterObj : public DiEmitterBaseObj
    {
    public:

        DiBoxEmitterObj();

        virtual              ~DiBoxEmitterObj();

    public:

        virtual DiString     GetType() override { return "BoxEmitter"; }

        virtual DiString     GetEmitterType() override { return "Box"; }

        DiBoxEmitter*        GetBoxEmitter();
    };
}

#endif
