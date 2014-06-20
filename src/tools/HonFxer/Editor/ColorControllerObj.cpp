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

#include "FxerPch.h"
#include "ColorControllerObj.h"
#include "HonFxerApp.h"
#include "EditorManager.h"
#include "EffectManager.h"
#include "ParticleElementObj.h"
#include "ParticleElement.h"
#include "ColorController.h"
#include "ControllerBaseObj.h"

namespace Demi
{
    DiColorControllerObj::DiColorControllerObj()
    {
    }

    DiColorControllerObj::~DiColorControllerObj()
    {
    }

    DiString DiColorControllerObj::GetUICaption()
    {
        return mController->GetName();
    }
    
    DiColorController* DiColorControllerObj::GetController()
    {
        return static_cast<DiColorController*>(mController);
    }

    void DiColorControllerObj::InitPropertyTable()
    {
        DiControllerBaseObj::InitPropertyTable();
        
        DiPropertyGroup* g = DI_NEW DiPropertyGroup("Color Controller");
        g->CreateUI();

        mPropGroups.push_back(g);
    }
}
