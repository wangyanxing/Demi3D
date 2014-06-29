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

#ifndef ControllerBaseObject_h__
#define ControllerBaseObject_h__

#include "FxerPrerequisites.h"
#include "BaseEditorObject.h"
#include "PropertyTypes.h"

namespace Demi 
{
    class DiControllerBaseObj : public DiBaseEditorObj
    {
    public:

        DiControllerBaseObj();

        virtual             ~DiControllerBaseObj();

    public:

        virtual void        OnMenuPopup(MyGUI::PopupMenu* menu, bool multiSelection) override;

        virtual void        OnCreate() override;
        
        virtual void        OnCreate(const DiAny& param);

        virtual void        OnDestroy() override;

        virtual void        OnSelect() override;

        virtual DiString    GetType() override { return "ControllerBase"; }

        virtual DiString    GetControllerType() = 0;

        virtual DiString    GetUICaption() override;

        virtual DiString    GetUINodeType() { return "File"; }

    public:

        DiParticleController*  GetController() { return mController; }

    protected:

        virtual void           InitPropertyTable() override;

    protected:

        DiParticleController*  mController{ nullptr };
    };
}

#endif
