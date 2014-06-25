#ifndef KyHandle_h__
#define KyHandle_h__

#include "KyDefines.h"

class KyHandle
{
public:
    KyHandle(int32 iIndex = -1, int32 iLogicID = -1, KyObjectType eObjectType = KOT_INVALID)
        : m_iIndex(iIndex)
        , m_iLogicID(iLogicID)
        , m_eObjectType(eObjectType){}

    KyHandle(const KyHandle& rkOther)
        : m_iIndex(rkOther.m_iIndex)
        , m_iLogicID(rkOther.m_iLogicID)
        , m_eObjectType(rkOther.m_eObjectType){}

    bool operator ==(const KyHandle& rkOther) const { return m_iLogicID == rkOther.m_iLogicID; }
    bool operator !=(const KyHandle& rkOther) const { return m_iLogicID != rkOther.m_iLogicID; }

    int32 GetIndex() const { return m_iIndex; }
    int32 GetLogicID() const { return m_iLogicID; }

private:
    int32          m_iIndex;
    int32          m_iLogicID;
    KyObjectType   m_eObjectType;
};

#endif

