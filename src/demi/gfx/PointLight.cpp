
#include "GfxPch.h"
#include "PointLight.h"
#include "GpuProgram.h"

namespace Demi 
{
    DiPointLight::DiPointLight(DiSceneManager* mgr)
        :DiLight(DiLight::TYPE_POINT,mgr),
        mPosition(DiVec3::ZERO),
        mAttenuationBegin(0),
        mAttenuationEnd(50)
    {
    }

    DiPointLight::~DiPointLight(void)
    {
    }

    const DiAABB& DiPointLight::GetBoundingBox( void ) const
    {
        return DiAABB::BOX_INFINITE;
    }

    DiString& DiPointLight::GetType()
    {
        static DiString type = "PointLight";
        return type;
    }
}