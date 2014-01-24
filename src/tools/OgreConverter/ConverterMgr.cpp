#include "ConverterMgr.h"
#include "AssetManager.h"
#include "OgreFileSystem.h"
#include "EnginePlugin.h"

using namespace Ogre;

#ifdef _DEBUG
const DiString gfxDrvLib = "DiDrvD3D9_d";
#else
const DiString gfxDrvLib = "DiDrvD3D9";
#endif

void ConverterMgr::Init()
{
	logMgr = new LogManager();

	// this log catches output from the parseArgs call and routes it to stdout only
	logMgr->createLog("Temporary log", false, true, true); 

	// use the log specified by the cmdline params
	logMgr->setDefaultLog(logMgr->createLog("MeshConverter.log", false, true)); 

	// get rid of the temporary log as we use the new log now
	logMgr->destroyLog("Temporary log");

	rgm = new ResourceGroupManager();
	mth = new Math();
	lodMgr = new LodStrategyManager();
	meshMgr = new MeshManager();
	matMgr = new MaterialManager();
	matMgr->initialise();
	skelMgr = new SkeletonManager();
	meshSerializer = new MeshSerializer();
	xmlMeshSerializer = new XMLMeshSerializer();
	skeletonSerializer = new SkeletonSerializer();
	xmlSkeletonSerializer = new XMLSkeletonSerializer();
	bufferManager = new DefaultHardwareBufferManager();
	archiveMgr = new ArchiveManager();

	FileSystemArchiveFactory* f = OGRE_NEW FileSystemArchiveFactory();
	ArchiveManager::getSingleton().addArchiveFactory( f );

	// philo trinity stuff

	DiLogManager* logmgr = new DiLogManager;
	logmgr->Init("DemiConverter.log");
    //DiPlugin::LoadPlugin(gfxDrvLib);

    DiAssetManager* asmager = new DiAssetManager();

	asmager->SetBasePath("..\\..\\Media\\");
}

void ConverterMgr::Shutdown()
{
	DiAssetManager* asmager = DiAssetManager::GetInstancePtr();
	delete asmager;

    //DiPlugin::UnloadPlugin(gfxDrvLib);

	DiLogManager* logmgr = DiLogManager::GetInstancePtr();
	delete logmgr;

	delete xmlSkeletonSerializer;
	delete skeletonSerializer;
	delete xmlMeshSerializer;
	delete meshSerializer;
	delete skelMgr;
	delete matMgr;
	delete meshMgr;
	delete bufferManager;
	delete lodMgr;
	delete mth;
	delete rgm;
	delete logMgr;
	delete archiveMgr;
}

ConverterMgr::ConverterMgr()
{
	logMgr = NULL;
	mth = NULL;
	lodMgr = NULL;
	matMgr = NULL;
	skelMgr = NULL;
	meshSerializer = NULL;
	xmlMeshSerializer = NULL;
	skeletonSerializer = NULL;
	xmlSkeletonSerializer = NULL;
	bufferManager = NULL;
	meshMgr = NULL;
	rgm = NULL;
	archiveMgr = NULL;

	Init();
}

ConverterMgr::~ConverterMgr()
{
	Shutdown();
}

void ConverterMgr::MeshToXML( CovOptions opts )
{
	std::ifstream ifs;
	ifs.open(opts.source.c_str(), std::ios_base::in | std::ios_base::binary);

	if (ifs.bad())
	{
		cout << "Unable to load file " << opts.source << endl;
		exit(1);
	}

	// pass false for freeOnClose to FileStreamDataStream since ifs is created on stack
	DataStreamPtr stream(new FileStreamDataStream(opts.source, &ifs, false));

	MeshPtr mesh = MeshManager::getSingleton().create("conversion", 
		ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);


	meshSerializer->importMesh(stream, mesh.getPointer());

	xmlMeshSerializer->exportMesh(mesh.getPointer(), opts.dest);
}

void ConverterMgr::SkeletonToXML( CovOptions opts )
{
	std::ifstream ifs;
	ifs.open(opts.source.c_str(), std::ios_base::in | std::ios_base::binary);
	if (ifs.bad())
	{
		cout << "Unable to load file " << opts.source << endl;
		exit(1);
	}

	SkeletonPtr skel = SkeletonManager::getSingleton().create("conversion", 
		ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

	// pass false for freeOnClose to FileStreamDataStream since ifs is created locally on stack
	DataStreamPtr stream(new FileStreamDataStream(opts.source, &ifs, false));
	skeletonSerializer->importSkeleton(stream, skel.getPointer());

	xmlSkeletonSerializer->exportSkeleton(skel.getPointer(), opts.dest);
}

void ConverterMgr::MeshToModel( CovOptions opts )
{
	std::ifstream ifs;
	ifs.open(opts.source.c_str(), std::ios_base::in | std::ios_base::binary);

	if (ifs.bad())
	{
		cout << "Unable to load file " << opts.source << endl;
		exit(1);
	}

	DataStreamPtr stream(new FileStreamDataStream(opts.source, &ifs, false));

	DiString inputname = opts.source.c_str();

	MeshPtr mesh = MeshManager::getSingleton().create(inputname.ExtractFileName().ExtractBaseName().c_str(), 
		ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

	meshSerializer->importMesh(stream, mesh.getPointer());

	modelConverter.ExportMesh(mesh.get(),opts.dest);
}

void ConverterMgr::SkeletonToMotion( CovOptions opts )
{
	std::ifstream ifs;
	ifs.open(opts.source.c_str(), std::ios_base::in | std::ios_base::binary);
	if (ifs.bad())
	{
		cout << "Unable to load file " << opts.source << endl;
		exit(1);
	}

	SkeletonPtr skel = SkeletonManager::getSingleton().create("conversion", 
		ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

	// pass false for freeOnClose to FileStreamDataStream since ifs is created locally on stack
	DataStreamPtr stream(new FileStreamDataStream(opts.source, &ifs, false));
	skeletonSerializer->importSkeleton(stream, skel.getPointer());

	modelConverter.ExportSkeleton(skel.get(),opts.dest);
}

CovOptions::CovOptions()
{
	includingXml = false;
	interactiveMode = false;
	nuextremityPoints = 0;
	usePercent = true;
	generateEdgeLists = true;
	generateTangents = false;
	tangentSemantic = VES_TANGENT;
	tangentUseParity = false;
	tangentSplitMirrored = false;
	tangentSplitRotated = false;
	reorganiseBuffers = true;
	optimiseAnimations = true;
	quietMode = false;
	endian = Serializer::ENDIAN_NATIVE;
}
