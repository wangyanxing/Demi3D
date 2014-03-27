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
        static ObjID_t Gen(){ return sID++; }
        static ObjID_t sID;
    };

    /** Entity manager
     */
    class DEMI_K2_API DiK2EntityManager
    {
    public:

        DiK2EntityManager();

        virtual     ~DiK2EntityManager();

    public:

        void                Update(float dt);

        DiK2HeroEntityPtr   CreateHero(ObjID_t id);

        ObjID_t             GetHeroID() const { return mHeroId; }

    private:

        typedef DiMap<ObjID_t, DiK2GameEntityPtr> EntityMap;
        EntityMap           mEntities;

        ObjID_t             mHeroId;

        DiK2HeroEntityPtr   mHeroEntity;
    };
}

#endif
