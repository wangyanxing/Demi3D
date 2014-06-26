#include "ArenaPch.h"
#include "KyMain.h"
#include "KyObjectMgr.h"

KyMain::KyMain()
{
	SetMain(this);

	mUnitMgr = new KyUnitMgr;
	mUnitMgr->SetMain(this);

	mEffectMgr = new KyEffectMgr;
	mEffectMgr->SetMain(this);
}

KyObject* KyMain::GetObj(const KyHandle& handle)
{
    switch (handle.GetObjType())
    {
    case KOT_UNIT: return mUnitMgr->GetObj(handle);
    case KOT_EFFECT: return mEffectMgr->GetObj(handle);
    default: return nullptr;
    }

    return nullptr;
}

void KyMain::Update(float deltaTime)
{
    mGameTime += deltaTime;

    mUnitMgr->Update(deltaTime);
    mEffectMgr->Update(deltaTime);

}