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

#ifndef PointEmitterObject_h__
#define PointEmitterObject_h__

#include "FxerPrerequisites.h"
#include "EmitterBaseObj.h"

namespace Demi 
{
    class DiPointEmitterObj : public DiEmitterBaseObj
    {
    public:

        DiPointEmitterObj();

        virtual              ~DiPointEmitterObj();

    public:

        virtual void         OnSelect() override;

        virtual DiString     GetType() override { return "PointEmitter"; }

        virtual DiString     GetEmitterType() override { return "Point"; }

        DiPointEmitter*      GetPointEmitter();
    };
}

#endif
