
/********************************************************************
    File:       OctreeQuery.h
    Creator:    demiwangya
*********************************************************************/

#pragma once

#include "SceneQuery.h"

namespace Demi
{
    class DI_GFX_API DiOcRayQuery : public DiRaySceneQuery
    {
    public:
        DiOcRayQuery(DiSceneManager* creator);
        ~DiOcRayQuery();

        void Execute(RaySceneQueryListener* listener);
    };

    class DI_GFX_API DiOcSphereQuery : public DiSphereSceneQuery
    {
    public:
        DiOcSphereQuery(DiSceneManager* creator);

        ~DiOcSphereQuery();

        void Execute(SceneQueryListener* listener);
    };
    
    class DI_GFX_API DiOcPBVQuery : public DiPBVListSceneQuery
    {
    public:
        DiOcPBVQuery(DiSceneManager* creator);
        ~DiOcPBVQuery();

        void Execute(SceneQueryListener* listener);
    };

    class DI_GFX_API DiOcAABBQuery : public DiAABBQuery
    {
    public:
        DiOcAABBQuery(DiSceneManager* creator);
        ~DiOcAABBQuery();

        void Execute(SceneQueryListener* listener);
    };
}