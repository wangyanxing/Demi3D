#ifndef KyObjectMgr_h__
#define KyObjectMgr_h__

#include "KyDefines.h"
#include "KyObject.h"

class KyObjectMgr : public KyMainContext
{
	KyDeclareRootRTTI(KyObjectMgr);
public:
    KyObjectMgr(){}
    virtual ~KyObjectMgr(){}

    KyObject* AddObject(uint32 uiTypeID);

protected:
    virtual KyObject* _NewObject(uint32 uiTypeID) = 0;
    virtual void      _DeleteObject(KyObject*& rpkObject) = 0;

private:
    std::vector<KyObject*>  m_akObjects;
    std::vector<int>        m_aiBlanks;
};


class KyUnitMgr : public KyObjectMgr
{
	KyDeclareRTTI;
public:
	KyUnitMgr(){}
	virtual ~KyUnitMgr(){}

protected:
	virtual KyObject* _NewObject(uint32 uiTypeID);
	virtual void      _DeleteObject(KyObject*& rpkObject);
};

class KyEffectMgr : public KyObjectMgr
{
	KyDeclareRTTI;
public:
	KyEffectMgr(){}
	virtual ~KyEffectMgr(){}

protected:
	virtual KyObject* _NewObject(uint32 uiTypeID);
	virtual void      _DeleteObject(KyObject*& rpkObject);
};

#endif

