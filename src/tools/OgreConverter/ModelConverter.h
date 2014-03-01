/**********************************************************************
This source file is a part of Demi3D
   __  ___  __  __  __
  |  \|_ |\/||   _)|  \ 
  |__/|__|  ||  __)|__/ 

Copyright (c) 2013-2014 Demi team
https://github.com/wangyanxing/Demi3D

Released under the MIT License
https://github.com/wangyanxing/Demi3D/blob/master/License.txt
***********************************************************************/

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

    void					WriteSimpleMaterial(const DiString& filename, const Ogre::String& ogreMatName);

	DiMeshPtr				WriteMesh(const Ogre::Mesh* pMesh);

	void					WriteSubMesh(DiSubMesh* subMesh, const Ogre::SubMesh* s);

	void					WriteSkeleton(const Ogre::Skeleton* pSkel, DiSkeleton* skeleton);

protected:

	Ogre::Mesh*				mMesh;

	Ogre::VertexElementType	mColourElementType;

    DiString                mCurrentOutputPath;
};



#endif
