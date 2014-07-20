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

#ifndef GravityControllerObject_h__
#define GravityControllerObject_h__

#include "K2Prerequisites.h"
#include "FxerPrerequisites.h"
#include "BaseEditorObject.h"
#include "ControllerBaseObj.h"
#include "PropertyTypes.h"

namespace Demi 
{
    class DiGravityControllerObj final : public DiControllerBaseObj
    {
    public:

        DiGravityControllerObj();

        ~DiGravityControllerObj();

    public:

        DiString    GetType() override { return "GravityController"; }

        DiString    GetControllerType() { return "Gravity"; }

        DiString    GetUICaption() override;

        DiString    GetUINodeType() { return "File"; }

    public:

        DiGravityController*  GetController();

    protected:

        void        InitPropertyTable() override;
    };
}

#endif
