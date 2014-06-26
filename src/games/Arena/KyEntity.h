#ifndef KyEntity_h__
#define KyEntity_h__

#include "KyDefines.h"
#include "KyObject.h"
#include "ArenaPropertyID.h"
#include "ArenaProperty.h"

/** For quick property definition
*/
#define KyDeclareProperty(Property)         \
public:                                     \
    Demi::Ar##Property* Get##Property()     \
    {                                       \
        DI_ASSERT(m##Property);             \
        return m##Property;                 \
    }                                       \
    const Demi::Ar##Property* Get##Property() const   \
    {                                       \
        DI_ASSERT(m##Property);             \
        return m##Property;                 \
    }                                       \
private:                                    \
    Demi::Ar##Property*  m##Property;


class KyEntity : public KyObject
{
	KyDeclareRTTI;
public:
	KyEntity(){}
    virtual ~KyEntity();

    template<typename T> T* CreateProperty()
    {
        T* pComp = DI_NEW T();
        mPropertyMap[T::GetStaticPropertyID()] = pComp;
        pComp->SetEntity(this);
        pComp->Activate();
        return pComp;
    }

    template<typename T> T* GetProperty()
    {
        auto it = mPropertyMap.find(T::GetStaticPropertyID());
        DI_ASSERT(it != mPropertyMap.end());
        return dynamic_cast<T*>(it->second);
    }

    void Init();

    virtual void InitComponents(){}

    virtual void InitAttribute(){}

    virtual void InitAttribute(const DiString& configFile){}

    template<typename EntityType>
    EntityType* GetEntity()
    {
        return dynamic_cast<EntityType*>(this);
    }

    virtual void Update(float deltaTime);

protected:
    typedef DiMap< Demi::ArPropertyIDType, Demi::ArProperty*> PropertyMap;
    PropertyMap mPropertyMap;

};

#endif
