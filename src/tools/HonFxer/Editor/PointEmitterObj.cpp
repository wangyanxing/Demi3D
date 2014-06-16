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
#include "PointEmitterObj.h"
#include "HonFxerApp.h"
#include "EditorManager.h"
#include "EffectManager.h"
#include "ParticleSystem.h"
#include "PointEmitter.h"

namespace Demi
{
    DiPointEmitterObj::DiPointEmitterObj()
    {
    }

    DiPointEmitterObj::~DiPointEmitterObj()
    {
    }

    void DiPointEmitterObj::OnSelect()
    {
    }

    DiPointEmitter* DiPointEmitterObj::GetPointEmitter()
    {
        return static_cast<DiPointEmitter*>(mEmitter);
    }
}