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

#ifndef DiK2EntityManager_h__
#define DiK2EntityManager_h__

#include "K2Prerequisites.h"
#include "K2Entity.h"
#include "K2GameDefines.h"

namespace Demi
{
    class DEMI_K2_API K2AutoID
    {
    public:
        static K2ObjID Gen(){ return sID++; }
        static K2ObjID sID;
    };

#define AUTO_ID K2AutoID::Gen()

    /** Entity manager
     */
    class DEMI_K2_API DiK2EntityManager
    {
    public:

        DiK2EntityManager();

        virtual     ~DiK2EntityManager();

    public:

        void                Update(float dt);

        /** usually the hero id is always 1
         */
        DiK2HeroEntityPtr   CreateHero(K2ObjID id);

        K2ObjID             GetHeroID() const { return mHeroId; }

    private:

        typedef DiMap<K2ObjID, DiK2GameEntityPtr> EntityMap;
        EntityMap           mEntities;

        K2ObjID             mHeroId;

        DiK2HeroEntityPtr   mHeroEntity;
    };
}

#endif
