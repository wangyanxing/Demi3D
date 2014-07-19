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
#include "BoxEmitterObj.h"
#include "HonFxerApp.h"
#include "EditorManager.h"
#include "EffectManager.h"
#include "ParticleSystem.h"
#include "BoxEmitter.h"

namespace Demi
{
    DiBoxEmitterObj::DiBoxEmitterObj()
    {
    }

    DiBoxEmitterObj::~DiBoxEmitterObj()
    {
    }

    DiBoxEmitter* DiBoxEmitterObj::GetBoxEmitter()
    {
        return static_cast<DiBoxEmitter*>(mEmitter);
    }
    
    void DiBoxEmitterObj::InitPropertyTable()
    {
        DiEmitterBaseObj::InitPropertyTable();
        
        DiPropertyGroup* g = DI_NEW DiPropertyGroup("Box Emitter");
        
        g->AddProperty("Box Width" , DI_NEW DiFloatProperty( [&]{ return GetBoxEmitter()->GetWidth(); },
                                                             [&](float& val){ GetBoxEmitter()->SetWidth(val); }));
        
        g->AddProperty("Box Height", DI_NEW DiFloatProperty( [&]{ return GetBoxEmitter()->GetHeight(); },
                                                            [&](float& val){ GetBoxEmitter()->SetHeight(val); }));
        
        g->AddProperty("Box Depth" , DI_NEW DiFloatProperty( [&]{ return GetBoxEmitter()->GetDepth(); },
                                                            [&](float& val){ GetBoxEmitter()->SetDepth(val); }));
        
        g->CreateUI();
        
        mPropGroups.push_back(g);

    }
}