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

#ifndef RefModelObject_h__
#define RefModelObject_h__

#include "FxerPrerequisites.h"
#include "BaseEditorObject.h"

namespace Demi 
{
    class DiRefModelObj : public DiBaseEditorObj
    {
    public:

        DiRefModelObj();

        virtual              ~DiRefModelObj();

    public:

        virtual void         OnMenuPopup(MyGUI::PopupMenu* menu, bool multiSelection) override;

        virtual void         OnCreate() override;
        
        virtual void         OnCreate(const DiAny& param) override;

        virtual void         OnDestroy() override;

        virtual void         OnSelect() override;

        virtual DiString     GetType() override { return "ReferenceModel"; }

        virtual DiString     GetUICaption() override;

        virtual void         Update(float dt);
        
        void                 SetModel(const DiString& file);

    public:

        DiK2ModelPtr         GetModel(){ return mModel; }
        
    protected:
        
        virtual void         InitPropertyTable();

    protected:

        DiK2ModelPtr         mModel;
        
        DiString             mName;
    };
}

#endif
