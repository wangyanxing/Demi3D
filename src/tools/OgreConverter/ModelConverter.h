#ifndef DiModelConverter_h__
#define DiModelConverter_h__


#include "Mesh.h"

class ModelConverter
{
public:
	ModelConverter(void);

	virtual					~ModelConverter(void);

public:

	void					ExportMesh(const Ogre::Mesh* pMesh, const Ogre::String& filename);

	void					ExportSkeleton(const Ogre::Skeleton* pSkeleton, const Ogre::String& filename);

protected:

	void					WriteSimpleMaterial( int boneWeightsNum, const DiString& filename );

	DiMeshPtr				WriteMesh(const Ogre::Mesh* pMesh);

	void					WriteSubMesh(DiSubMesh* subMesh, const Ogre::SubMesh* s);

	void					WriteSkeleton(const Ogre::Skeleton* pSkel, DiSkeleton* skeleton);

protected:

	Ogre::Mesh*				mMesh;

	Ogre::VertexElementType	mColourElementType;
};



#endif
