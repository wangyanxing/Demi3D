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

#ifndef ArenaLevel_h__
#define ArenaLevel_h__

#include "ArenaPrerequisites.h"
#include "ArenaConfigs.h"

namespace Demi
{
    /** Game level
     */
    class ArLevel : public DiBase
    {
    public:

        ArLevel();

        ~ArLevel();

    public:

        void LoadMap(const DiString& configfile);

        void Update(float dt);

        ArConfigMap& GetConfig() { return mMapConfig; }

        DiK2World* GetWorld() { return mWorld; }

        DiVec3 GetSpwanPosition();

    private:

        ArConfigMap mMapConfig;

        DiK2World* mWorld;
    };
}

#endif
