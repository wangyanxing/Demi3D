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

#ifndef VortexControllerObject_h__
#define VortexControllerObject_h__

#include "FxPrerequisites.h"
#include "FxerPrerequisites.h"
#include "BaseEditorObject.h"
#include "ControllerBaseObj.h"
#include "PropertyTypes.h"

namespace Demi 
{
    class DiVortexControllerObj final : public DiControllerBaseObj
    {
    public:

        DiVortexControllerObj();

        ~DiVortexControllerObj();

    public:

        DiString    GetType() override { return "VortexController"; }

        DiString    GetControllerType() { return "Vortex"; }

        DiString    GetUICaption() override;

        DiString    GetUINodeType() { return "File"; }

    public:

        DiVortexController*  GetController();

    protected:

        void        InitPropertyTable() override;
    };
}

#endif
