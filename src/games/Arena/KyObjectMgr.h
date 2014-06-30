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

    void Update(float deltaTime);

    KyObject* AddObj(uint32 typeID);
    KyObject* GetObj(const KyHandle& handle);

protected:
    virtual KyObject* _NewObj(uint32 typeID) = 0;

    KyObjectType      mObjectType{ KOT_INVALID };

private:
    std::vector<KyObject*>  mObjects;
    std::vector<size_t>     mBlankIdx;
};


class KyHeroMgr : public KyObjectMgr
{
	KyDeclareRTTI;
public:
    KyHeroMgr() : KyObjectMgr(KOT_HERO) {}
    virtual ~KyHeroMgr(){}

protected:
    virtual KyObject* _NewObj(uint32 typeID);
};

class KyNpcMgr : public KyObjectMgr
{
	KyDeclareRTTI;
public:
    KyNpcMgr() : KyObjectMgr(KOT_NPC) {}
    virtual ~KyNpcMgr(){}

protected:
    virtual KyObject* _NewObj(uint32 typeID);
};

#endif

