#include "ArenaPch.h"
#include "KyMain.h"
#include "KyObjectMgr.h"

KyMain::KyMain()
{
	SetMain(this);

    mHeroMgr = new KyHeroMgr;
    mHeroMgr->SetMain(this);

    mNpcMgr = new KyNpcMgr;
    mNpcMgr->SetMain(this);
}

KyObject* KyMain::GetObj(const KyHandle& handle)
{
    switch (handle.GetObjType())
    {
    case KOT_HERO: return mHeroMgr->GetObj(handle);
    case KOT_NPC: return mNpcMgr->GetObj(handle);
    default: return nullptr;
    }

    return nullptr;
}

void KyMain::Update(float deltaTime)
{
    mGameTime += deltaTime;

    mHeroMgr->Update(deltaTime);
    mNpcMgr->Update(deltaTime);

}