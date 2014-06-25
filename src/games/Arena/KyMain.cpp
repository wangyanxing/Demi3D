#include "ArenaPch.h"
#include "KyMain.h"
#include "KyObjectMgr.h"

KyMain::KyMain()
{
	SetMain(this);

	m_pkUnitMgr = new KyUnitMgr;
	m_pkUnitMgr->SetMain(this);

	m_pkEffectMgr = new KyEffectMgr;
	m_pkEffectMgr->SetMain(this);
}

KyObject* KyMain::GetObj(const KyHandle& rkHandle)
{
	return nullptr;
}