
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