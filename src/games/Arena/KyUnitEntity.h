#ifndef KyUnitEntity_h__
#define KyUnitEntity_h__

#include "KyDefines.h"
#include "KyObject.h"
#include "ArenaPropertyID.h"
#include "ArenaProperty.h"

/** For quick property definition
*/
#define KyDeclareProperty(Property)         \
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


class KyUnitEntity : public KyObject
{
	KyDeclareRTTI;
public:
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

public:
    KyUnitEntity(){}
    virtual ~KyUnitEntity();

    void Init();

    virtual void InitComponents(){}
    virtual void InitAttribute(){}
    virtual void InitAttribute(const DiString& configFile){}

    virtual void Update(float deltaTime);

    DiK2RenderObject*  GetRenderObj() { return mRenderObj; }

protected:
    typedef DiMap< ArPropertyIDType, ArProperty*> PropertyMap;
    PropertyMap        mPropertyMap;

    DiK2RenderObject*  mRenderObj { nullptr };
};

#endif
