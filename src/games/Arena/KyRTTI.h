#ifndef KyRTTI_h__
#define KyRTTI_h__

class  KyRTTI
{
public:
	KyRTTI(const char* pcName, const KyRTTI* pkBaseRTTI)
		: m_pcName(pcName), m_pkBaseRTTI(pkBaseRTTI){}

	const char* GetName() const { return m_pcName; }
	const KyRTTI* GetBaseRTTI() const { return m_pkBaseRTTI; }

protected:
	const char* m_pcName;
	const KyRTTI* m_pkBaseRTTI;
};

// insert in root class declaration
#define KyDeclareRootRTTI(classname) \
	public: \
	static const KyRTTI ms_RTTI; \
	virtual const KyRTTI* GetRTTI() const { return &ms_RTTI; } \
	static bool IsExactKindOf(const KyRTTI* pkRTTI, const classname* pkObject) \
	{ \
		if (!pkObject) \
		{ \
			return false; \
		} \
		return pkObject->IsExactKindOf(pkRTTI); \
	} \
	bool IsExactKindOf(const KyRTTI* pkRTTI) const \
	{ \
		return (GetRTTI() == pkRTTI); \
	} \
	static bool IsKindOf(const KyRTTI* pkRTTI, const classname* pkObject) \
	{ \
		if (!pkObject) \
		{ \
			return false; \
		} \
		return pkObject->IsKindOf(pkRTTI); \
	} \
	bool IsKindOf(const KyRTTI* pkRTTI) const \
	{ \
		const KyRTTI* pkTmp = GetRTTI(); \
		while (pkTmp) \
		{ \
			if (pkTmp == pkRTTI) \
			{ \
				return true; \
			} \
			pkTmp = pkTmp->GetBaseRTTI(); \
		} \
		return false; \
	} \
	static classname* DynamicCast(const KyRTTI* pkRTTI, const classname* pkObject) \
	{ \
		if (!pkObject) \
		{ \
			return 0; \
		} \
		return pkObject->DynamicCast(pkRTTI); \
	} \
	classname* DynamicCast(const KyRTTI* pkRTTI) const \
	{ \
		return (IsKindOf(pkRTTI) ? (classname*) this : 0); \
	}

// insert in class declaration
#define KyDeclareRTTI \
	public: \
	static const KyRTTI ms_RTTI; \
	virtual const KyRTTI* GetRTTI() const { return &ms_RTTI; }

// insert in root class source file
#define KyImplementRootRTTI(classname) \
	const KyRTTI classname::ms_RTTI(#classname, 0)

// insert in class source file
#define KyImplementRTTI(classname, baseclassname) \
	const KyRTTI classname::ms_RTTI(#classname, &baseclassname::ms_RTTI)

// macros for run-time type testing
#define KyIsExactKindOf(classname, pkObject) \
	classname::IsExactKindOf(&classname::ms_RTTI, pkObject)

#define KyIsKindOf(classname, pkObject) \
	classname::IsKindOf(&classname::ms_RTTI, pkObject)

// macro for run-time type casting
#define KyStaticCast(classname, pkObject) \
	((classname*)pkObject)

#define KyDynamicCast(classname, pkObject) \
	((classname*)classname::DynamicCast(&classname::ms_RTTI, pkObject))

#endif
