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

#ifndef DiK2Entity_h__
#define DiK2Entity_h__

#include "K2Prerequisites.h"
#include "K2PropertyID.h"

namespace Demi
{
    /** For quick property definition
     */
#define DEF_DECLARE_PRIORITY(Property)      \
public:                                     \
    DiK2##Property* Get##Property()         \
    {                                       \
        DI_ASSERT(m##Property);             \
        return m##Property;                 \
    }                                       \
    const DiK2##Property* Get##Property() const   \
    {                                       \
        DI_ASSERT(m##Property);             \
        return m##Property;                 \
    }                                       \
private:                                    \
    DiK2##Property*  m##Property;


    /** a base entity class
     */
    class DEMI_K2_API DiK2Entity : public DiBase
    {
    public:

        friend class DiK2EntityManager;

        template<typename T>
        T* CreateProperty()
        {
            T* pComp = DI_NEW T();
            mPropertyMap[T::GetStaticPropertyID()] = pComp;
            pComp->SetEntity(this);
            pComp->Activate();
            return pComp;
        }

        template<typename T>
        T* GetProperty()
        {
            auto it = mPropertyMap.find(T::GetStaticPropertyID());
            DI_ASSERT(it != mPropertyMap.end());
            return dynamic_cast<T*>(it->second);
        }

        virtual void InitComponents(){}

        virtual void InitAttribute(){}

        template<typename EntityType>
        EntityType* GetEntity()
        {
            return dynamic_cast<EntityType*>(this);
        }

        K2ObjID GetID() const { return mID; }

    protected:
        void SetID(K2ObjID id) { mID = id; }

        typedef DiMap< K2PropertyIDType, DiK2Property*> PropertyMap;
        PropertyMap mPropertyMap;

        K2ObjID mID;

    public:
        
        DiK2Entity();
        
        virtual ~DiK2Entity();

        virtual void Update(float dt);
    };
}

#endif
