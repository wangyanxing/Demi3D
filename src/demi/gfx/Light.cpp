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
#include "SceneManager.h"
#include "Node.h"

namespace Demi 
{
    DiLight::DiLight(Type type,DiSceneManager* mgr)
        :mType(type),
        mColor(DiColor::White),
        mCreator(mgr),
        mCameraToBeRelativeTo(NULL),
        mDerivedCamRelativeDirty(false),
        mDerivedTransformDirty(false)
    {
    }

    DiLight::~DiLight(void)
    {
    }

    void DiLight::SetColor( const DiColor &color )
    {
        mColor = color;
    }

    void DiLight::Update( void ) const
    {
        
    }

}