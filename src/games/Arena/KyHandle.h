#ifndef KyHandle_h__
#define KyHandle_h__

#include "KyDefines.h"

class KyHandle
{
    friend class KyRefHandle;
public:
    KyHandle(size_t index = npos, uint32 logicID = (~0), KyObjectType objectType = KOT_INVALID)
        : mIndex(index)
        , mLogicID(logicID)
        , mObjectType(objectType){}

    KyHandle(const KyHandle& other)
        : mIndex(other.mIndex)
        , mLogicID(other.mLogicID)
        , mObjectType(other.mObjectType){}

    bool operator ==(const KyHandle& other) const { return mLogicID == other.mLogicID; }
    bool operator !=(const KyHandle& other) const { return mLogicID != other.mLogicID; }

    size_t       GetIndex() const { return mIndex; }
    uint32       GetLogicID() const { return mLogicID; }
    KyObjectType GetObjType() const { return mObjectType; }

private:
    size_t         mIndex;
    uint32         mLogicID;
    KyObjectType   mObjectType;
};

#endif

