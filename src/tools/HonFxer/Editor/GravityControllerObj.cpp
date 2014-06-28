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
#include "GravityControllerObj.h"
#include "HonFxerApp.h"
#include "EditorManager.h"
#include "EffectManager.h"
#include "ParticleElementObj.h"
#include "ParticleElement.h"
#include "GravityController.h"
#include "ControllerBaseObj.h"

namespace Demi
{
    DiGravityControllerObj::DiGravityControllerObj()
    {
    }

    DiGravityControllerObj::~DiGravityControllerObj()
    {
    }

    DiString DiGravityControllerObj::GetUICaption()
    {
        return mController->GetName();
    }
    
    DiGravityController* DiGravityControllerObj::GetController()
    {
        return static_cast<DiGravityController*>(mController);
    }

    void DiGravityControllerObj::InitPropertyTable()
    {
        DiControllerBaseObj::InitPropertyTable();
        
        DiPropertyGroup* g = DI_NEW DiPropertyGroup("Gravity Controller");
        
        g->AddProperty("Gravity" , DI_NEW DiFloatProperty([&]{ return GetController()->GetGravity(); },
                                                          [&](float& val){ GetController()->SetGravity(val); }));
        
        g->CreateUI();

        mPropGroups.push_back(g);
    }
}
