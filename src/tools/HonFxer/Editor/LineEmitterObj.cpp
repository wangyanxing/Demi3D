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

    void DiLineEmitterObj::OnSelect()
    {
    }

    DiLineEmitter* DiLineEmitterObj::GetLineEmitter()
    {
        return static_cast<DiLineEmitter*>(mEmitter);
    }

}