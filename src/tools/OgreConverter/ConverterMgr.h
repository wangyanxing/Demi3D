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

#ifndef DiConverterMgr_h__
#define DiConverterMgr_h__

#include "ModelConverter.h"

using Ogre::String;
using Ogre::Real;

struct CovOptions
{
	CovOptions();
	String source;
	String dest;
	String logFile;
	bool interactiveMode;

	size_t nuextremityPoints;
	bool usePercent;
	bool generateEdgeLists;
	bool generateTangents;
	Ogre::VertexElementSemantic tangentSemantic;
	bool tangentUseParity;
	bool tangentSplitMirrored;
	bool tangentSplitRotated;
	bool reorganiseBuffers;
	bool optimiseAnimations;
	bool quietMode;
	Ogre::Serializer::Endian endian;
	bool includingXml;
};

class ConverterMgr
{
public:

	ConverterMgr();

	~ConverterMgr();

	void								Init();

	void								Shutdown();

	void								MeshToXML(CovOptions opts);

	void								SkeletonToXML(CovOptions opts);

	void								MeshToModel(CovOptions opts);

	void								SkeletonToMotion(CovOptions opts);

protected:

	Ogre::LogManager*					logMgr;

	Ogre::Math*							mth;

	Ogre::LodStrategyManager*			lodMgr;

	Ogre::MaterialManager*				matMgr;

	Ogre::SkeletonManager*				skelMgr;

	Ogre::MeshSerializer*				meshSerializer;

	Ogre::XMLMeshSerializer*			xmlMeshSerializer;

	Ogre::SkeletonSerializer*			skeletonSerializer;

	Ogre::XMLSkeletonSerializer*		xmlSkeletonSerializer;

    Ogre::DefaultHardwareBufferManager* bufferManager;

	Ogre::MeshManager*					meshMgr;

	Ogre::ResourceGroupManager*			rgm;

	Ogre::ArchiveManager*				archiveMgr;

	ModelConverter						modelConverter;
};



#endif
