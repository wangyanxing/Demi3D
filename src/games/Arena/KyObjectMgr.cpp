#include "ArenaPch.h"
#include "KyObjectMgr.h"

KyImplementRootRTTI(KyObjectMgr);
KyImplementRTTI(KyUnitMgr,KyObjectMgr);
KyImplementRTTI(KyEffectMgr,KyObjectMgr);

KyObject* KyObjectMgr::AddObject(uint32 uiTypeID)
{
    KyObject* pkObject = _NewObject(uiTypeID);
    if (!pkObject)
    {
        return nullptr;
    }

	pkObject->SetMain(GetMain());
	return nullptr;
}

KyObject* KyUnitMgr::_NewObject(uint32 uiTypeID)
{
	return nullptr;
}

void KyUnitMgr::_DeleteObject(KyObject*& rpkObject)
{

}

KyObject* KyEffectMgr::_NewObject(uint32 uiTypeID)
{
	return nullptr;
}

void KyEffectMgr::_DeleteObject(KyObject*& rpkObject)
{

}