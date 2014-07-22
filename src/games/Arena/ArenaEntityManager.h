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

#ifndef ArenaEntityManager_h__
#define ArenaEntityManager_h__

#include "ArenaPrerequisites.h"
#include "ArenaEntity.h"
#include "ArenaGameDefines.h"
#include "KyMain.h"

namespace Demi
{
    /** Entity manager
     */
    class ArEntityManager
    {
    public:

        ArEntityManager();

        virtual     ~ArEntityManager();

    public:

        void                Update(float dt);

        /** usually the hero id is always 1
         */
        ArHeroEntityPtr     CreateHero(ArObjID id, const DiString& configfile);

        ArGameEntityPtr     CreateNPC(ArObjID id, const DiString& configfile);

        static ArObjID      GetHeroID() { return mHeroId; }

        ArGameEntityPtr     FindEntity(ArObjID id);

        ArHeroEntityPtr     GetHero() { return mHeroEntity; }

        bool                IsHero(ArObjID id) { return id == mHeroId; }
        
        // Get or load an entity config structure
        // e.g. GetEntityConfig("heroes/aluna/hero.entity");
        ArEntityConfigs*    GetEntityConfig(const DiString& name);

    private:

        void                Release();
        
        ArEntityConfigs*    LoadEntityConfig(const DiString& name);

    private:

        typedef DiMap<ArObjID, ArGameEntityPtr> EntityMap;
        EntityMap           mEntities;

        static ArObjID      mHeroId;

        ArHeroEntityPtr     mHeroEntity;
        
        DiStrHash<ArEntityConfigs*> mEntityConfigs;
    };
}

#endif
