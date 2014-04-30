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

#ifndef ArenaEntity_h__
#define ArenaEntity_h__

#include "ArenaPrerequisites.h"
#include "ArenaPropertyID.h"

namespace Demi
{
    /** For quick property definition
     */
#define DEF_DECLARE_PRIORITY(Property)      \
public:                                     \
    Ar##Property* Get##Property()           \
    {                                       \
        DI_ASSERT(m##Property);             \
        return m##Property;                 \
    }                                       \
    const Ar##Property* Get##Property() const   \
    {                                       \
        DI_ASSERT(m##Property);             \
        return m##Property;                 \
    }                                       \
private:                                    \
    Ar##Property*  m##Property;


    /** a base entity class
     */
    class ArEntity : public DiBase
    {
    public:

        friend class ArEntityManager;

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

        ArObjID GetID() const { return mID; }

    protected:
        void SetID(ArObjID id) { mID = id; }

        typedef DiMap< ArPropertyIDType, ArProperty*> PropertyMap;
        PropertyMap mPropertyMap;

        ArObjID mID;

    public:
        
        ArEntity();
        
        virtual ~ArEntity();

        virtual void Update(float dt);
    };
}

#endif
