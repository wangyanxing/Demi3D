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

#ifndef EmitterBaseObject_h__
#define EmitterBaseObject_h__

#include "FxerPrerequisites.h"
#include "BaseEditorObject.h"
#include "PropertyTypes.h"

namespace Demi 
{
    class DiEmitterBaseObj : public DiBaseEditorObj
    {
    public:

        DiEmitterBaseObj();

        virtual             ~DiEmitterBaseObj();

    public:

        virtual void        OnMenuPopup(MyGUI::PopupMenu* menu, bool multiSelection) override;

        virtual void        OnCreate() override;

        virtual void        OnDestroy() override;

        virtual void        OnSelect() override;

        virtual DiString    GetType() override { return "EmitterBase"; }

        virtual DiString    GetEmitterType() { return ""; }

        virtual DiString    GetUICaption() override;

        virtual DiString    GetUINodeType() { return "File"; }

    public:

        DiParticleEmitter*  GetEmitter() { return mEmitter; }

    protected:

        virtual void        InitPropertyTable() override;

    protected:

        DiParticleEmitter*  mEmitter{ nullptr };
    };
}

#endif
