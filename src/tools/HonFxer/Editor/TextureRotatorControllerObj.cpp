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
#include "TextureRotatorControllerObj.h"
#include "HonFxerApp.h"
#include "EditorManager.h"
#include "EffectManager.h"
#include "ParticleElementObj.h"
#include "ParticleElement.h"
#include "TextureRotatorController.h"
#include "ControllerBaseObj.h"

namespace Demi
{
    DiTextureRotatorControllerObj::DiTextureRotatorControllerObj()
    {
    }

    DiTextureRotatorControllerObj::~DiTextureRotatorControllerObj()
    {
    }

    DiString DiTextureRotatorControllerObj::GetUICaption()
    {
        return mController->GetName();
    }
    
    DiTextureRotatorController* DiTextureRotatorControllerObj::GetController()
    {
        return static_cast<DiTextureRotatorController*>(mController);
    }

    void DiTextureRotatorControllerObj::InitPropertyTable()
    {
        DiControllerBaseObj::InitPropertyTable();
        
        DiPropertyGroup* g = DI_NEW DiPropertyGroup("Texture Rotator Controller");
        
        g->AddProperty("Own Speed"   , DI_NEW DiBoolProperty([&]{ return GetController()->UseOwnRotationSpeed(); },
                                                                      [&](bool& val){ GetController()->SetUseOwnRotationSpeed(val); }));
        
        g->AddProperty("Rotation"             , DI_NEW DiDynProperty( [&]{ return GetController()->GetRotation(); },
                                                                      [&](DiDynamicAttribute*& val){ GetController()->SetRotation(val); }));
        
        g->AddProperty("Rotation Speed"       , DI_NEW DiDynProperty( [&]{ return GetController()->GetRotationSpeed(); },
                                                                      [&](DiDynamicAttribute*& val){ GetController()->SetRotationSpeed(val); }));
        
        g->CreateUI();

        mPropGroups.push_back(g);
    }
}
