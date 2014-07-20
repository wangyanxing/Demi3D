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

#ifndef RandomiserControllerObject_h__
#define RandomiserControllerObject_h__

#include "K2Prerequisites.h"
#include "FxerPrerequisites.h"
#include "BaseEditorObject.h"
#include "ControllerBaseObj.h"
#include "PropertyTypes.h"

namespace Demi 
{
    class DiRandomiserControllerObj final : public DiControllerBaseObj
    {
    public:

        DiRandomiserControllerObj();

        ~DiRandomiserControllerObj();

    public:

        DiString    GetType() override { return "RandomiserController"; }

        DiString    GetControllerType() { return "Randomiser"; }

        DiString    GetUICaption() override;

        DiString    GetUINodeType() { return "File"; }

    public:

        DiRandomiserController*  GetController();

    protected:

        void        InitPropertyTable() override;
    };
}

#endif
