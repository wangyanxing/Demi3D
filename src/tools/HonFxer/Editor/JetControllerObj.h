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

#ifndef JetControllerObject_h__
#define JetControllerObject_h__

#include "FxPrerequisites.h"
#include "FxerPrerequisites.h"
#include "BaseEditorObject.h"
#include "ControllerBaseObj.h"
#include "PropertyTypes.h"

namespace Demi 
{
    class DiJetControllerObj final : public DiControllerBaseObj
    {
    public:

        DiJetControllerObj();

        ~DiJetControllerObj();

    public:

        DiString    GetType() override { return "JetController"; }

        DiString    GetControllerType() { return "Jet"; }

        DiString    GetUICaption() override;

        DiString    GetUINodeType() { return "File"; }

    public:

        DiJetController*  GetController();

    protected:

        void        InitPropertyTable() override;
    };
}

#endif
