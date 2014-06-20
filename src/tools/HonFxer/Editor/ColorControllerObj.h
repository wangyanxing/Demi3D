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

#ifndef ColorControllerObject_h__
#define ColorControllerObject_h__

#include "FxPrerequisites.h"
#include "FxerPrerequisites.h"
#include "BaseEditorObject.h"
#include "ControllerBaseObj.h"
#include "PropertyTypes.h"

namespace Demi 
{
    class DiColorControllerObj final : public DiControllerBaseObj
    {
    public:

        DiColorControllerObj();

        ~DiColorControllerObj();

    public:

        DiString    GetType() override { return "ColorController"; }

        DiString    GetControllerType() { return "Color"; }

        DiString    GetUICaption() override;

        DiString    GetUINodeType() { return "File"; }

    public:

        DiColorController*  GetController();

    protected:

        void        InitPropertyTable() override;
    };
}

#endif
