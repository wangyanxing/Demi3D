
#include "GfxPch.h"
#include "SkyLight.h"
#include "GpuProgram.h"

namespace Demi 
{
    DiSkyLight::DiSkyLight(DiSceneManager* mgr)
        :DiLight(DiLight::TYPE_SKY,mgr),
        mPosition(DiVec3::ZERO)
    {
    }

    DiSkyLight::~DiSkyLight(void)
    {
    }

    const DiAABB& DiSkyLight::GetBoundingBox(void) const
    {
        return DiAABB::BOX_INFINITE;
    }

    DiString& DiSkyLight::GetType()
    {
        static DiString type = "SkyLight";
        return type;
    }
}