#ifndef KyEntity_h__
#define KyEntity_h__

#include "KyDefines.h"
#include "KyObject.h"

class KyEntity : public KyObject
{
	KyDeclareRTTI;
public:
	KyEntity(){}
	virtual ~KyEntity(){}

};

#endif
