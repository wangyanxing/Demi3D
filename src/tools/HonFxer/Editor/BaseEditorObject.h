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

#ifndef BaseEditorObject_h__
#define BaseEditorObject_h__

#include "FxerPrerequisites.h"

namespace Demi 
{
    class DiBaseEditorObj
    {
    public:

        DiBaseEditorObj();

        virtual ~DiBaseEditorObj();

    public:

        template<typename T>
        T* CreateChild(const DiString& type)
        {
            return dynamic_cast<T*>(_CreateChild(type));
        }

        DiBaseEditorObj* GetParent() { return mParent; }

        virtual void OnMenuPopup(MyGUI::PopupMenu* menu, bool multiSelection);

    protected:

        virtual DiBaseEditorObj* _CreateChild(const DiString& type);

    protected:

        DiVector<DiBaseEditorObj*> mChildren;

        DiBaseEditorObj* mParent{ nullptr };
    };
}

#endif
