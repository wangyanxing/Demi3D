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
    class DEMI_K2_API DiK2Entity
    {
    public:

        friend class DiK2EntityManager;

        template<typename T>
        T* createProperty()
        {
            T* pComp = new T();
            mPropertyMap[T::GetStaticPropertyID()] = pComp;
            pComp->setEntity(this);
            pComp->activate();
            return pComp;
        }

        template<typename T>
        T* getProperty()
        {
            auto it = mPropertyMap.find(T::GetStaticPropertyID());
            DI_ASSERT(it != mPropertyMap.end());
            return dynamic_cast<T*>(it->second);
        }

        virtual void initComponents()
        {
        }

        virtual void initAttribute()
        {
        }

        template<typename EntityType>
        EntityType* getEntity()
        {
            return dynamic_cast<EntityType*>(this);
        }

        ObjID_t getID() const { return mID; }

    protected:
        void setID(ObjID_t id) { mID = id; }

        typedef DiMap< ENUM_PROPERTY_ID_TYPE, DiK2Property*> PropertyMap;
        PropertyMap mPropertyMap;

        ObjID_t mID;

    public:
        
        DiK2Entity();
        
        virtual ~DiK2Entity();

        virtual void update(float dt);
    };
}

#endif
