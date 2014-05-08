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

        virtual void        Update(float dt);

        virtual void        InitComponents(){}

        template<typename T>
        void                SetAttribute()
        {
            SAFE_DELETE(mAttribute);
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

    protected:

        DiK2RenderObject*   mRenderObj{ nullptr };

        ArAttribute*        mAttribute{ nullptr };

        K2ObjTypes          mType{ MAX_GAME_ENTITY_TYPES };

        DiString            mModel;
    };
}

#endif
