#ifndef KyObjectMgr_h__
#define KyObjectMgr_h__

#include "KyDefines.h"
#include "KyObject.h"

class KyObjectMgr : public KyMainContext
{
	KyDeclareRootRTTI(KyObjectMgr);
public:
    KyObjectMgr(KyObjectType eObjectType = KOT_INVALID)
    {
        mObjectType = eObjectType;
    }

    virtual ~KyObjectMgr(){}

    void Update(uint32 deltaTime);

    KyObject* AddObj(uint32 typeID);
    KyObject* GetObj(const KyHandle& handle);

protected:
    virtual KyObject* _NewObj(uint32 typeID) = 0;

    KyObjectType      mObjectType{ KOT_INVALID };

private:
    std::vector<KyObject*>  mObjects;
    std::vector<size_t>     mBlankIdx;
};


class KyUnitMgr : public KyObjectMgr
{
	KyDeclareRTTI;
public:
    KyUnitMgr() : KyObjectMgr(KOT_UNIT) {}
	virtual ~KyUnitMgr(){}

protected:
    virtual KyObject* _NewObj(uint32 typeID);
};

class KyEffectMgr : public KyObjectMgr
{
	KyDeclareRTTI;
public:
    KyEffectMgr() : KyObjectMgr(KOT_EFFECT) {}
	virtual ~KyEffectMgr(){}

protected:
    virtual KyObject* _NewObj(uint32 typeID);
};

#endif

