#ifndef KyRefHandle_h__
#define KyRefHandle_h__

#include "KyDefines.h"
#include "KyHandle.h"

class KyRefHandle : public KyHandle, public KyMainContext
{
public:
    KyRefHandle(){}
    ~KyRefHandle();
    KyRefHandle(const KyHandle& handle, KyMain* mainContext);
    KyRefHandle(const KyRefHandle& other);
    KyRefHandle& operator =(const KyRefHandle& other);

private:
    void _Copy(const KyRefHandle& other);
};

#endif

