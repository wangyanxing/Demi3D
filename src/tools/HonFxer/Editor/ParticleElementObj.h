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

#ifndef ParticleElementObject_h__
#define ParticleElementObject_h__

#include "FxerPrerequisites.h"
#include "BaseEditorObject.h"

namespace Demi 
{
    class DiParticleElementObj : public DiBaseEditorObj
    {
    public:

        DiParticleElementObj();

        virtual              ~DiParticleElementObj();

    public:

        virtual void         OnMenuPopup(MyGUI::PopupMenu* menu, bool multiSelection) override;

        virtual void         OnCreate() override;

        virtual void         OnDestroy() override;

        virtual void         OnSelect() override;

        virtual DiString     GetType() override { return "ParticleElement"; }

        virtual DiString     GetUICaption() override;
        
        void                 SetTexture(DiTexturePtr texture);
        
        DiTexturePtr         GetTexture();

    public:

        DiParticleElement*   GetParticleElement() { return mParticleElement; }

    protected:
        
        virtual void         InitPropertyTable();

        DiParticleElement*   mParticleElement{ nullptr };
    };
}

#endif
