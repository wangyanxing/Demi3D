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
        
        DiPropertyGroup* g = DI_NEW DiPropertyGroup("Vortex Controller");
        
        g->AddProperty("Rotation Vector"            , DI_NEW DiVec3Property([&]{ return GetController()->GetRotationVector(); },
                                                                            [&](DiVec3& val){ GetController()->SetRotationVector(val); }));
        
        g->AddProperty("Rotation Speed"             , DI_NEW DiDynProperty( [&]{ return GetController()->GetRotationSpeed(); },
                                                                            [&](DiDynamicAttribute*& val){ GetController()->SetRotationSpeed(val); }));
        
        g->CreateUI();

        mPropGroups.push_back(g);
    }
}
