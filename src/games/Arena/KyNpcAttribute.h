#ifndef KyUnitAttribute_h__
#define KyUnitAttribute_h__

#include "KyAttribute.h"

class KyUnitAttribute : public KyAttribute
{
public:
    KyUnitAttribute(){}
    virtual ~KyUnitAttribute(){}

public:
    virtual void LoadAttribute(const DiXMLElement& node){}
};

#endif