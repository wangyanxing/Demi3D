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

#ifndef ArenaGameEntity_h__
#define ArenaGameEntity_h__

#include "ArenaPrerequisites.h"
#include "ArenaEntity.h"
#include "ArenaGameDefines.h"

#define DEFAULT_FOLLOWER_NUM 5

namespace Demi
{
    class ArGameEntity : public ArEntity
    {
    public:

        ArGameEntity();

        virtual ~ArGameEntity();

    public:

        void                SetModel(const DiString& model);

        void                Init(K2ObjTypes type);

        DiK2RenderObject*   GetRenderObj() { return mRenderObj; }

        K2ObjTypes          GetType() const { return mType; }

        virtual void        Update(float dt) override;

        virtual void        InitComponents() override {}

        /** Check the distance between two entities
            including both radius

            return false if far from the specific distance
         */
        bool                CheckDistance(ArGameEntity* entity, float distance);

        bool                CheckDistance(ArObjID target, float distance);

        /** Check the pure distance between two entities' origin 
            return false if far from the specific distance
        */
        bool                CheckAbsDistance(ArGameEntity* entity, float distance);

        bool                CheckAbsDistance(ArObjID target, float distance);

        template<typename T>
        void                SetAttribute()
        {
            DI_ASSERT(!mAttribute);
            mAttribute = DI_NEW T(mID);
        }

        ArAttribute*        GetAttribute(){ return mAttribute; }

        template<typename T>
        T*  GetAttribute()
        {
            DI_ASSERT(mAttribute);
            return static_cast<T*>(mAttribute);
        }

        bool                IsDead();
        
        void                AddFollower(ArObjID follower, const DiK2Pos& pos);
        
        DiK2Pos             GetFollowTarget(ArObjID follower, float radius);
        
        void                RemoveFollower(ArObjID follower);

    protected:

        DiK2RenderObject*   mRenderObj{ nullptr };

        ArAttribute*        mAttribute{ nullptr };

        K2ObjTypes          mType{ MAX_GAME_ENTITY_TYPES };

        DiString            mModel;
        
        DiVector<ArObjID>   mFollowers{ DEFAULT_FOLLOWER_NUM, INVALID_OBJ_ID };
        
#if 1
        DiDebugHelperPtr    mDebugger;
#endif
    };
}

#endif
