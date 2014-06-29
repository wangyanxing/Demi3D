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
#include "ScaleControllerObj.h"
#include "HonFxerApp.h"
#include "EditorManager.h"
#include "EffectManager.h"
#include "ParticleElementObj.h"
#include "ParticleElement.h"
#include "ScaleController.h"
#include "ControllerBaseObj.h"

namespace Demi
{
    DiScaleControllerObj::DiScaleControllerObj()
    {
    }

    DiScaleControllerObj::~DiScaleControllerObj()
    {
    }

    DiString DiScaleControllerObj::GetUICaption()
    {
        return mController->GetName();
    }
    
    DiScaleController* DiScaleControllerObj::GetController()
    {
        return static_cast<DiScaleController*>(mController);
    }

    void DiScaleControllerObj::InitPropertyTable()
    {
        DiControllerBaseObj::InitPropertyTable();
        
        DiPropertyGroup* g = DI_NEW DiPropertyGroup("Scale Controller");
        
        g->AddProperty("Since Start System"  , DI_NEW DiBoolProperty([&]{ return GetController()->IsSinceStartSystem(); },
                                                                     [&](bool& val){ GetController()->SetSinceStartSystem(val); }));
        
        g->AddProperty("Begin Scale"         , DI_NEW DiVec2Property([&]{ return GetController()->GetBeginScale(); },
                                                                     [&](DiVec2& val){ GetController()->SetBeginScale(val); }));
        
        g->AddProperty("Median Scale"        , DI_NEW DiVec2Property([&]{ return GetController()->GetMedianScale(); },
                                                                     [&](DiVec2& val){ GetController()->SetMedianScale(val); }));
        
        g->AddProperty("End Scale"           , DI_NEW DiVec2Property([&]{ return GetController()->GetEndScale(); },
                                                                     [&](DiVec2& val){ GetController()->SetEndScale(val); }));
        
        g->CreateUI();

        mPropGroups.push_back(g);
    }
}
