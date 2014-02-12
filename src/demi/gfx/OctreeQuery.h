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

#ifndef DiOctreeQuery_h__
#define DiOctreeQuery_h__


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

#endif
