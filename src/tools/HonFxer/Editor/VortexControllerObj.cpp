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
#include "VortexControllerObj.h"
#include "HonFxerApp.h"
#include "EditorManager.h"
#include "EffectManager.h"
#include "ParticleElementObj.h"
#include "ParticleElement.h"
#include "VortexController.h"
#include "ControllerBaseObj.h"

namespace Demi
{
    DiVortexControllerObj::DiVortexControllerObj()
    {
    }

    DiVortexControllerObj::~DiVortexControllerObj()
    {
    }

    DiString DiVortexControllerObj::GetUICaption()
    {
        return mController->GetName();
    }
    
    DiVortexController* DiVortexControllerObj::GetController()
    {
        return static_cast<DiVortexController*>(mController);
    }

    void DiVortexControllerObj::InitPropertyTable()
    {
        DiControllerBaseObj::InitPropertyTable();
        
        auto vortexCtrl = GetController();
        
        DiPropertyGroup* g = DI_NEW DiPropertyGroup("Vortex Controller");
        
        g->AddProperty("Rotation Vector"            , DI_NEW DiVec3Property([&]{ return vortexCtrl->GetRotationVector(); },
                                                                            [&](DiVec3& val){ vortexCtrl->SetRotationVector(val); }));
        
        g->AddProperty("Rotation Speed"             , DI_NEW DiDynProperty( [&]{ return vortexCtrl->GetRotationSpeed(); },
                                                                            [&](DiDynamicAttribute*& val){ vortexCtrl->SetRotationSpeed(val); }));
        
        g->CreateUI();

        mPropGroups.push_back(g);
    }
}
