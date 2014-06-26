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

    void      Update(uint32 deltaTime);

private:
    uint32		 mLogicIDGen{ 0 };

	KyUnitMgr*	 mUnitMgr{ nullptr };
	KyEffectMgr* mEffectMgr{ nullptr };
};

#endif
