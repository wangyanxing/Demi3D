#include "ArenaPch.h"
#include "KyObjectMgr.h"
#include "KyMain.h"

KyImplementRootRTTI(KyObjectMgr);
KyImplementRTTI(KyUnitMgr,KyObjectMgr);
KyImplementRTTI(KyEffectMgr,KyObjectMgr);

KyObject* KyObjectMgr::AddObj(uint32 typeID)
{
    KyObject* pkObj = _NewObj(typeID);
    if (!pkObj)
    {
        return nullptr;
    }

    pkObj->SetMain(GetMain());

    uint32 logicID = GetMain()->NewLogicID();

    size_t index;
    if (mBlankIdx.size() > 0)
    {
        index = mBlankIdx.back();
        mBlankIdx.pop_back();
    }
    else
    {
        mObjects.push_back(nullptr);
        index = mObjects.size() - 1;
    }

    mObjects[index] = pkObj;

    pkObj->_SetHandle(KyHandle(index, logicID, mObjectType));

    return pkObj;
}

KyObject* KyObjectMgr::GetObj(const KyHandle& handle)
{
    size_t index = handle.GetIndex();
    if (index < mObjects.size())
    {
        KyObject* pkObj = mObjects[index];
        if (pkObj &&
            !(pkObj->GetCanRemove() && pkObj->GetRefCount() <= 0) &&
            pkObj->GetHandle().GetLogicID() == handle.GetLogicID())
        {
            return pkObj;
        }
    }
    return nullptr;
}

void KyObjectMgr::Update(float deltaTime)
{
    for (size_t i = 0; i < mObjects.size(); ++i)
    {
        KyObject*& object = mObjects[i];

        if (object &&
            object->GetCanRemove() &&
            object->GetRefCount() <= 0)
        {
            delete object;
            object = nullptr;
            mBlankIdx.push_back(i);
        }
        else
        {
            object->Update(deltaTime);
        }
    }
}

KyObject* KyUnitMgr::_NewObj(uint32 typeID)
{
	return nullptr;
}

KyObject* KyEffectMgr::_NewObj(uint32 typeID)
{
	return nullptr;
}
