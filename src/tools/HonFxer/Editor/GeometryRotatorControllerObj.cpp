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
#include "GeometryRotatorControllerObj.h"
#include "HonFxerApp.h"
#include "EditorManager.h"
#include "EffectManager.h"
#include "ParticleElementObj.h"
#include "ParticleElement.h"
#include "GeometryRotatorController.h"
#include "ControllerBaseObj.h"

namespace Demi
{
    DiGeometryRotatorControllerObj::DiGeometryRotatorControllerObj()
    {
    }
    
    DiGeometryRotatorControllerObj::~DiGeometryRotatorControllerObj()
    {
    }
    
    DiString DiGeometryRotatorControllerObj::GetUICaption()
    {
        return mController->GetName();
    }
    
    DiGeometryRotatorController* DiGeometryRotatorControllerObj::GetController()
    {
        return static_cast<DiGeometryRotatorController*>(mController);
    }
    
    void DiGeometryRotatorControllerObj::InitPropertyTable()
    {
        DiControllerBaseObj::InitPropertyTable();
        
        DiPropertyGroup* g = DI_NEW DiPropertyGroup("Geometry Rotator Controller");
        
        g->AddProperty("Own Speed"   , DI_NEW DiBoolProperty([&]{ return GetController()->UseOwnRotationSpeed(); },
                                                                      [&](bool& val){ GetController()->SetUseOwnRotationSpeed(val); }));
        
        g->AddProperty("Rotation Axis"        , DI_NEW DiVec3Property([&]{ return GetController()->GetRotationAxis(); },
                                                                      [&](DiVec3& val){ GetController()->SetRotationAxis(val); }));
        
        g->AddProperty("Rotation Speed"       , DI_NEW DiDynProperty( [&]{ return GetController()->GetRotationSpeed(); },
                                                                     [&](DiDynamicAttribute*& val){ GetController()->SetRotationSpeed(val); }));
        
        g->CreateUI();
        
        mPropGroups.push_back(g);
    }
}
