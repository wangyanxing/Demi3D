#include "ArenaPch.h"
#include "KyObjectMgr.h"



KyObject* KyObjectMgr::AddObject(uint32 uiTypeID)
{
    KyObject* pkObject = _NewObject(uiTypeID);
    if (!pkObject)
    {
        return nullptr;
    }

    pkObject->SetLogicMain(GetLogicMain());

}