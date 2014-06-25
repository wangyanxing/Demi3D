#ifndef KyMain_h__
#define KyMain_h__

#include "KyDefines.h"
#include "KyHandle.h"

class KyMain : public KyMainContext
{
public:
    KyMain();
    virtual ~KyMain() {}

	KyObject* GetObj(const KyHandle& rkHandle);

    int32     NewLogicID() { return (++m_iLogicIDGen); }


private:
    int32		 m_iLogicIDGen{ 0 };

	KyUnitMgr*	 m_pkUnitMgr{ nullptr };
	KyEffectMgr* m_pkEffectMgr{ nullptr };
};

#endif
