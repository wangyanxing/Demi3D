#include "ArenaPch.h"
#include "KyUnitEntity.h"

KyImplementRTTI(KyUnitEntity, KyObject);

KyUnitEntity::~KyUnitEntity()
{
    auto it = mPropertyMap.begin();
    auto itEnd = mPropertyMap.end();
    while (it != itEnd)
    {
        it->second->Deactivate();
        DI_DELETE(it->second);
        it++;
    }
    mPropertyMap.clear();
}

void KyUnitEntity::Update(float deltaTime)
{
    auto it = mPropertyMap.begin();
    auto itEnd = mPropertyMap.end();
    while (it != itEnd)
    {
        it->second->Update(deltaTime);
        it++;
    }
}

void KyUnitEntity::Init()
{
    //auto world = ArGameApp::Get()->GetWorld();
    //mRenderObj = world->CreateRenderObject(type);
}
