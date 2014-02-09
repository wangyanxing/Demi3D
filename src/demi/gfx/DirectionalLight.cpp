#include "GfxPch.h"
#include "DirectionalLight.h"
#include "GpuProgram.h"

namespace Demi 
{
    DiDirLight::DiDirLight(DiSceneManager* mgr)
        :DiLight(DiLight::TYPE_DIRECTIONAL,mgr),
        mDirection(DiVec3::UNIT_Z)
    {
    }

    DiDirLight::~DiDirLight(void)
    {
    }

    void DiDirLight::Bind( DiShaderEnvironment* env )
    {
    }

    const DiAABB& DiDirLight::GetBoundingBox( void ) const
    {
        return DiAABB::BOX_INFINITE;
    }

    DiString& DiDirLight::GetType()
    {
        static DiString type = "DirectionalLight";
        return type;
    }
}