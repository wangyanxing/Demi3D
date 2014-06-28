#ifndef KyAttribute_h__
#define KyAttribute_h__

#include "KyDefines.h"

class KyAttribute : public KyMainContext
{
public:
    KyAttribute(){}
    virtual ~KyAttribute(){}

public:
    virtual void LoadAttribute(const DiXMLElement& node) = 0;
};

#endif