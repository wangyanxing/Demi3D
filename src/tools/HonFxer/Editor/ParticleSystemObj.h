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

#ifndef ParticleSystemObject_h__
#define ParticleSystemObject_h__

#include "FxerPrerequisites.h"
#include "BaseEditorObject.h"

namespace Demi 
{
    class DiParticleSystemObj : public DiBaseEditorObj
    {
    public:

        DiParticleSystemObj();

        virtual              ~DiParticleSystemObj();

    public:

        virtual void         OnMenuPopup(MyGUI::PopupMenu* menu, bool multiSelection) override;

        virtual void         OnCreate() override;
        
        virtual void         OnCreate(const DiAny& param) override;

        virtual void         OnDestroy() override;

        virtual void         OnSelect() override;

        virtual DiString     GetType() override { return "ParticleSystem"; }

        virtual DiString     GetUICaption() override;

        virtual void         Update(float dt);

    public:

        DiParticleSystemPtr  GetParticleSystem() { return mParticleSystem; }
        
    protected:
        
        virtual void         InitPropertyTable();

    protected:

        DiParticleSystemPtr  mParticleSystem{ nullptr };
    };
}

#endif
