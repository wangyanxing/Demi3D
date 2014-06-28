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
#include "ForceControllerObj.h"
#include "HonFxerApp.h"
#include "EditorManager.h"
#include "EffectManager.h"
#include "ParticleElementObj.h"
#include "ParticleElement.h"
#include "SineForceController.h"
#include "LinearForceController.h"
#include "ControllerBaseObj.h"
#include "EnumProperties.h"

namespace Demi
{
    DiBaseForceControllerObj::DiBaseForceControllerObj()
    {
    }

    DiBaseForceControllerObj::~DiBaseForceControllerObj()
    {
    }

    DiString DiBaseForceControllerObj::GetUICaption()
    {
        return mController->GetName();
    }
    
    DiBaseForceController* DiBaseForceControllerObj::GetController()
    {
        return static_cast<DiBaseForceController*>(mController);
    }

    void DiBaseForceControllerObj::InitBasePropertyTable()
    {
        DiControllerBaseObj::InitPropertyTable();
        
        DiPropertyGroup* g = DI_NEW DiPropertyGroup("Force Controller");
        
        using AppMode = DiBaseForceController::ForceApplication;
        
        g->AddProperty("Application Mode",   DI_NEW DiEnumProperty([&](){ return make_shared<ForceApplicationEnum>(GetController()->GetForceApplication()); },
                                                             [&](DiBaseEnumPropPtr& val){
                                                                 GetController()->SetForceApplication(val->getEnum<ForceApplicationEnum,AppMode>());}));
        
        g->AddProperty("Force Vector", DI_NEW DiVec3Property([&](){ return GetController()->GetForceVector(); },
                                                             [&](DiVec3& val){ GetController()->SetForceVector(val);}));
        
        g->CreateUI();

        mPropGroups.push_back(g);
    }
    
    DiLinearForceController* DiLinearForceControllerObj::GetLinearForce()
    {
        return static_cast<DiLinearForceController*>(mController);
    }
    
    void DiLinearForceControllerObj::InitPropertyTable()
    {
        InitBasePropertyTable();
    }
    
    DiSineForceController* DiSineForceControllerObj::GetSineForce()
    {
        return static_cast<DiSineForceController*>(mController);
    }
 
    void DiSineForceControllerObj::InitPropertyTable()
    {
        InitBasePropertyTable();
        
        DiPropertyGroup* g = DI_NEW DiPropertyGroup("Sine Force");
        
        g->AddProperty("Min Frequency",   DI_NEW DiFloatProperty([&](){ return GetSineForce()->GetFrequencyMin(); },
                                                                 [&](float& val){ GetSineForce()->SetFrequencyMin(val);}));
        
        g->AddProperty("Max Frequency",   DI_NEW DiFloatProperty([&](){ return GetSineForce()->GetFrequencyMax(); },
                                                                 [&](float& val){ GetSineForce()->SetFrequencyMax(val);}));
        
        g->CreateUI();
        
        mPropGroups.push_back(g);

    }
}
