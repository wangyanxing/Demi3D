#ifndef KyMain_h__
#define KyMain_h__

#include "KyDefines.h"
#include "KyHandle.h"

class KyMain : public KyMainContext
{
public:
    KyMain();
    virtual ~KyMain() {}

    KyObject* GetObj(const KyHandle& handle);

    uint32    NewLogicID() { return (++mLogicIDGen); }

    void      Update(float deltaTime);

private:
    uint32		 mLogicIDGen{ 0 };

    float        mGameTime{ 0.f };

    KyPlayer     mPlayer[MAX_PLAYER_COUNT];

    KyUnitMgr*	 mUnitMgr{ nullptr };
    KyEffectMgr* mEffectMgr{ nullptr };
};

#endif
