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

    const KyHandle& GetHandle() const { return mHandle; }
    int32           GetRefCount() const { return mRefCount; }
    void            SetCanRemove(bool bCanRemove) { mCanRemove = bCanRemove; }
    bool            GetCanRemove() const { return mCanRemove; }

    virtual void    Update(uint32 deltaTime) = 0;

private:
    void      _SetHandle(const KyHandle& rkHandle) { mHandle = rkHandle; }
    void      _IncRefCount() { ++mRefCount; }
    void      _DecRefCount() { --mRefCount; }

private:
    KyHandle  mHandle;
    int32     mRefCount{ 0 };
    bool      mCanRemove{ false };
};

#endif
