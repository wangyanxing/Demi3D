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

namespace Demi
{
    class ArAutoID
    {
    public:
        static ArObjID Gen(){ return sID++; }
        static ArObjID sID;
    };

#define AUTO_ID ArAutoID::Gen()

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

        ArObjID             GetHeroID() const { return mHeroId; }

        ArGameEntityPtr     FindEntity(ArObjID id);

    private:

        void                Release();

    private:

        typedef DiMap<ArObjID, ArGameEntityPtr> EntityMap;
        EntityMap           mEntities;

        ArObjID             mHeroId { INVALID_OBJ_ID };

        ArHeroEntityPtr     mHeroEntity;
    };
}

#endif
