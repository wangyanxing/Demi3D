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
#include "LineEmitterObj.h"
#include "HonFxerApp.h"
#include "EditorManager.h"
#include "EffectManager.h"
#include "ParticleSystem.h"
#include "LineEmitter.h"

namespace Demi
{
    DiLineEmitterObj::DiLineEmitterObj()
    {
    }

    DiLineEmitterObj::~DiLineEmitterObj()
    {
    }

    DiLineEmitter* DiLineEmitterObj::GetLineEmitter()
    {
        return static_cast<DiLineEmitter*>(mEmitter);
    }
    
    void DiLineEmitterObj::InitPropertyTable()
    {
        DiEmitterBaseObj::InitPropertyTable();
        
        DiPropertyGroup* g = DI_NEW DiPropertyGroup("Line Emitter");
        
        g->AddProperty("End" , DI_NEW DiVec3Property( [&]{ return GetLineEmitter()->GetEnd(); },
                                                      [&](DiVec3& val){ GetLineEmitter()->SetEnd(val); }));
        
        g->AddProperty("Min Increment", DI_NEW DiFloatProperty( [&]{ return GetLineEmitter()->GetMinIncrement(); },
                                                                [&](float& val){ GetLineEmitter()->SetMinIncrement(val); }));
        
        g->AddProperty("Max Increment", DI_NEW DiFloatProperty( [&]{ return GetLineEmitter()->GetMaxIncrement(); },
                                                                [&](float& val){ GetLineEmitter()->SetMaxIncrement(val); }));
        
        g->AddProperty("Max Deviation", DI_NEW DiFloatProperty( [&]{ return GetLineEmitter()->GetMaxDeviation(); },
                                                                [&](float& val){ GetLineEmitter()->SetMaxDeviation(val); }));
        
        g->CreateUI();
        
        mPropGroups.push_back(g);
    }
}