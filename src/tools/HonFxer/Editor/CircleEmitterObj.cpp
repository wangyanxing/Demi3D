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
#include "CircleEmitterObj.h"
#include "HonFxerApp.h"
#include "EditorManager.h"
#include "EffectManager.h"
#include "ParticleSystem.h"
#include "CircleEmitter.h"

namespace Demi
{
    DiCircleEmitterObj::DiCircleEmitterObj()
    {
    }

    DiCircleEmitterObj::~DiCircleEmitterObj()
    {
    }

    void DiCircleEmitterObj::OnSelect()
    {
    }

    DiCircleEmitter* DiCircleEmitterObj::GetCircleEmitter()
    {
        return static_cast<DiCircleEmitter*>(mEmitter);
    }

}