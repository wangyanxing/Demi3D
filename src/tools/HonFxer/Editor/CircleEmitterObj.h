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

#ifndef CircleEmitterObject_h__
#define CircleEmitterObject_h__

#include "FxerPrerequisites.h"
#include "EmitterBaseObj.h"

namespace Demi 
{
    class DiCircleEmitterObj : public DiEmitterBaseObj
    {
    public:

        DiCircleEmitterObj();

        virtual              ~DiCircleEmitterObj();

    public:

        virtual DiString     GetType() override { return "CircleEmitter"; }

        virtual DiString     GetEmitterType() override { return "Circle"; }

        DiCircleEmitter*     GetCircleEmitter();
    };
}

#endif
