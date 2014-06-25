#ifndef KyObject_h__
#define KyObject_h__

#include "KyDefines.h"
#include "KyHandle.h"

class KyObject : public KyMainContext
{
	KyDeclareRootRTTI(KyObject);

    friend class KyRefHandle;
    friend class KyObjectMgr;
public:
    KyObject(){}
    virtual ~KyObject(){}

    const KyHandle& GetHandle() const { return m_kHandle; }
    int             GetRefCount() const { return m_iRefCount; }
	void            SetCanRemove(bool bCanRemove) { m_bCanRemove = bCanRemove; }
    bool            GetCanRemove() const { return m_bCanRemove; }

private:
    void      _SetHandle(const KyHandle& rkHandle) { m_kHandle = rkHandle; }
    void      _IncRefCount() { ++m_iRefCount; }
    void      _DecRefCount() { --m_iRefCount; }

private:
    KyHandle  m_kHandle;
    int       m_iRefCount{ 0 };
    bool      m_bCanRemove{ false };
};

#endif
