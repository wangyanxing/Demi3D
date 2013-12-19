
/********************************************************************
    File:       DirectionalLight.h
    Creator:    demiwangya
*********************************************************************/

#pragma once

#include "light.h"

namespace Demi 
{
    class DI_GFX_API DiDirLight : public DiLight
    {
    public:

        friend class DiSceneManager;

    protected:

        DiDirLight(DiSceneManager* mgr);

        virtual ~DiDirLight();

    public:

        virtual void             Bind(DiShaderEnvironment* env);

        void                     SetDirection(const DiVec3& vec){
            mDirection = vec;
        }

        DiVec3                   GetDirection() const {return mDirection;}

        virtual const DiAABB&    GetBoundingBox(void) const;

        virtual DiString&        GetType();

    protected:

        DiVec3                   mDirection;
        
    };

}