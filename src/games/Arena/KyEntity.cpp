#include "ArenaPch.h"
#include "KyEntity.h"

KyImplementRTTI(KyEntity,KyObject);

KyEntity::~KyEntity()
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

void KyEntity::Update(float deltaTime)
{
    auto it = mPropertyMap.begin();
    auto itEnd = mPropertyMap.end();
    while (it != itEnd)
    {
        it->second->Update(deltaTime);
        it++;
    }
}

void KyEntity::Init()
{
    //auto world = ArGameApp::Get()->GetWorld();
    //mRenderObj = world->CreateRenderObject(type);
}
