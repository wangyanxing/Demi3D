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

#ifndef DiK2GameEntity_h__
#define DiK2GameEntity_h__

#include "K2Prerequisites.h"
#include "K2Entity.h"
#include "K2GameDefines.h"

namespace Demi
{
    class DEMI_K2_API DiK2GameEntity : public DiK2Entity
    {
    public:

        DiK2GameEntity();

        virtual ~DiK2GameEntity();

    public:

        void                SetModel(const DiString& model);

        void                Init(K2ObjTypes type);

        DiK2RenderObject*   GetRenderObj() { return mRenderObj; }

        K2ObjTypes          GetType() const { return mType; }

        virtual void        InitComponents(){}

        template<typename T>
        void                SetAttribute()
        {
            mAttribute = make_shared<T>();
        }

        DiK2AttributePtr    GetAttribute(){ return mAttribute; }

        template<typename T>
        std::shared_ptr<T>  GetAttribute()
        {
            DI_ASSERT(mAttribute);
            return std::dynamic_pointer_cast<T>(mAttribute);
        }

    protected:

        DiK2RenderObject*   mRenderObj;

        DiK2AttributePtr    mAttribute;

        K2ObjTypes          mType;

        DiString            mModel;
    };
}

#endif
