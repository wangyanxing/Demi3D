#include "ArenaPch.h"
#include "KyRefHandle.h"
#include "KyMain.h"
#include "KyObject.h"

KyRefHandle::KyRefHandle(const KyHandle& handle, KyMain* mainContext)
: KyHandle(handle)
, KyMainContext(mainContext)
{
    DI_ASSERT(GetMain());
    KyObject* pkObj = GetMain()->GetObj(*this);
    if (pkObj)
    {
        pkObj->_IncRefCount();
    }
}

KyRefHandle::~KyRefHandle()
{
    DI_ASSERT(GetMain());
    KyObject* pkObj = GetMain()->GetObj(*this);
    if (pkObj)
    {
        pkObj->_DecRefCount();
    }
}

KyRefHandle::KyRefHandle(const KyRefHandle& other)
{
    _Copy(other);
}

KyRefHandle& KyRefHandle::operator =(const KyRefHandle& other)
{
    _Copy(other);
    return (*this);
}

void KyRefHandle::_Copy(const KyRefHandle& other)
{
    DI_ASSERT(GetMain());

    KyObject* pkObj = GetMain()->GetObj(*this);
    if (pkObj)
    {
        pkObj->_DecRefCount();
    }

    SetMain(other.GetMain());
    mIndex = other.mIndex;
    mLogicID = other.mIndex;
    mObjectType = other.mObjectType;

    pkObj = GetMain()->GetObj(*this);
    if (pkObj)
    {
        pkObj->_IncRefCount();
    }
}


