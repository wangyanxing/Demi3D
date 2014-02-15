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
#include "GfxPch.h"
#include "Light.h"

namespace Demi 
{
    DiLight::DiLight(LightType type)
        :mType(type),
        mColor(DiColor::White)
    {
    }

    DiLight::~DiLight(void)
    {
    }
    
    DiString& DiLight::GetType()
    {
        static DiString type = "Light";
        return type;
    }
}