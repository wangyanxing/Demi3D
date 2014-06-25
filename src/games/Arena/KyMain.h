#ifndef KyMain_h__
#define KyMain_h__

#include "KyDefines.h"

class KyMain : public KyMainContext
{
public:
    KyMain();
    virtual ~KyMain() {}

    int32  NewLogicID() { return (++m_iLogicIDGen); }


private:
    int32               m_iLogicIDGen { 0 };

    //ArEntityManager*    m_pkEntityMgr { nullptr };

};

#endif
