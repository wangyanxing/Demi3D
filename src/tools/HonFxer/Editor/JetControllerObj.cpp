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
#include "JetControllerObj.h"
#include "HonFxerApp.h"
#include "EditorManager.h"
#include "EffectManager.h"
#include "ParticleElementObj.h"
#include "ParticleElement.h"
#include "JetController.h"
#include "ControllerBaseObj.h"

namespace Demi
{
    DiJetControllerObj::DiJetControllerObj()
    {
    }

    DiJetControllerObj::~DiJetControllerObj()
    {
    }

    DiString DiJetControllerObj::GetUICaption()
    {
        return mController->GetName();
    }
    
    DiJetController* DiJetControllerObj::GetController()
    {
        return static_cast<DiJetController*>(mController);
    }

    void DiJetControllerObj::InitPropertyTable()
    {
        DiControllerBaseObj::InitPropertyTable();
        
        DiPropertyGroup* g = DI_NEW DiPropertyGroup("Jet Controller");
    
        g->AddProperty("Acceleration" , DI_NEW DiDynProperty( [&]{ return GetController()->GetDynAcceleration(); },
                                                              [&](DiDynamicAttribute*& val){ GetController()->SetDynAcceleration(val); }));
        
        g->CreateUI();

        mPropGroups.push_back(g);
    }
}
