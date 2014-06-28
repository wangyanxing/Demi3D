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
#include "RandomiserControllerObj.h"
#include "HonFxerApp.h"
#include "EditorManager.h"
#include "EffectManager.h"
#include "ParticleElementObj.h"
#include "ParticleElement.h"
#include "RandomiserController.h"
#include "ControllerBaseObj.h"

namespace Demi
{
    DiRandomiserControllerObj::DiRandomiserControllerObj()
    {
    }

    DiRandomiserControllerObj::~DiRandomiserControllerObj()
    {
    }

    DiString DiRandomiserControllerObj::GetUICaption()
    {
        return mController->GetName();
    }
    
    DiRandomiserController* DiRandomiserControllerObj::GetController()
    {
        return static_cast<DiRandomiserController*>(mController);
    }

    void DiRandomiserControllerObj::InitPropertyTable()
    {
        DiControllerBaseObj::InitPropertyTable();
        
        DiPropertyGroup* g = DI_NEW DiPropertyGroup("Randomiser Controller");
        
        g->AddProperty("Max Deviation X" , DI_NEW DiFloatProperty([&]{ return GetController()->GetMaxDeviationX(); },
                                                                  [&](float& val){ GetController()->SetMaxDeviationX(val); }));
        
        g->AddProperty("Max Deviation Y" , DI_NEW DiFloatProperty([&]{ return GetController()->GetMaxDeviationY(); },
                                                                  [&](float& val){ GetController()->SetMaxDeviationY(val); }));
        
        g->AddProperty("Max Deviation Z" , DI_NEW DiFloatProperty([&]{ return GetController()->GetMaxDeviationZ(); },
                                                                  [&](float& val){ GetController()->SetMaxDeviationZ(val); }));
        
        g->AddProperty("Time Step"       , DI_NEW DiFloatProperty([&]{ return GetController()->GetTimeStep(); },
                                                                  [&](float& val){ GetController()->SetTimeStep(val); }));
        
        g->AddProperty("Random Direction", DI_NEW DiBoolProperty( [&]{ return GetController()->IsRandomDirection(); },
                                                                  [&](bool& val){ GetController()->SetRandomDirection(val); }));
        
        g->CreateUI();

        mPropGroups.push_back(g);
    }
}
