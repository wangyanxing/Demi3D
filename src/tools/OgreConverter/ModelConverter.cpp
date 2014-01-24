#include "ModelConverter.h"
#include "Mesh.h"
#include "SubMesh.h"
#include "AssetManager.h"
#include "MeshSerial.h"
#include "Motion.h"
#include "MotionSerial.h"
#include "Skeleton.h"
#include "Bone.h"
#include "KeyFrame.h"
#include "AnimationClip.h"
#include "ClipController.h"
#include "Animation.h"
#include "Material.h"
#include "MaterialSerial.h"
#include "ShaderParam.h"
#include "ShaderProgram.h"
#include "MaterialPredefine.h"

using namespace Ogre;

static DiVertexType ConverteVertType(VertexElementType type)
{
	switch(type)
	{
	case VET_FLOAT1:
		return VERT_TYPE_FLOAT1;
	case VET_FLOAT2:
        return VERT_TYPE_FLOAT2;
	case VET_FLOAT3:
        return VERT_TYPE_FLOAT3;
	case VET_FLOAT4:
        return VERT_TYPE_FLOAT4;
	case VET_COLOUR:
		return VERT_TYPE_COLOR;
	case VET_SHORT1:
		DI_ERROR("Unsupported type");
        return VERT_TYPE_UNUSED;
	case VET_SHORT2:
        return VERT_TYPE_SHORT2;
	case VET_SHORT3:
		DI_ERROR("Unsupported type");
        return VERT_TYPE_UNUSED;
	case VET_SHORT4:
        return VERT_TYPE_SHORT4;
	case VET_UBYTE4:
        return VERT_TYPE_UBYTE4;
	case VET_COLOUR_ARGB:
        return VERT_TYPE_COLOR;
	case VET_COLOUR_ABGR:
	default:
		DI_ERROR("Unsupported type");
        return VERT_TYPE_UNUSED;
	}
}

ModelConverter::ModelConverter(void)
	:mMesh(NULL)
{
}

ModelConverter::~ModelConverter(void)
{
}

void ModelConverter::ExportMesh( const Mesh* pMesh, const String& filename )
{
	LogManager::getSingleton().logMessage("XMLMeshSerializer writing mesh data to " + filename + "...");
	
	mMesh = const_cast<Mesh*>(pMesh);

	LogManager::getSingleton().logMessage("Populating DOM...");

	DiMeshPtr mesh = WriteMesh(pMesh);

	LogManager::getSingleton().logMessage("DOM populated, writing model file..");

	if (mesh)
	{
		// save to file
		DiMeshSerializer ms;
		ms.ExportMesh(mesh,filename);

		// write material files
		DiMesh::SubMeshIterator it = mesh->GetSubMeshs();
		while (it.HasMoreElements())
		{
			DiSubMesh* subMesh = it.GetNext();
			unsigned int maxw = subMesh->GetMaxWeights();
			DiString matname = subMesh->GetMaterialName();

			DiString matfile = filename.c_str();
			matfile = matfile.ExtractDirName();
			matfile += matname;

			WriteSimpleMaterial(maxw,matfile);
		}
	}
}

DiMeshPtr ModelConverter::WriteMesh( const Ogre::Mesh* pMesh )
{
	DiMeshPtr model = Demi::DiAssetManager::GetInstancePtr(
		)->CreateOrReplaceAsset<DiMesh>(pMesh->getName().c_str());

	// write AABB
	AxisAlignedBox aabb = pMesh->getBounds();
	DiAABB maabb;
	maabb.SetExtents(aabb.getMinimum().x,aabb.getMinimum().y,aabb.getMinimum().z,
		aabb.getMaximum().x,aabb.getMaximum().y,aabb.getMaximum().z);
	model->SetBounds(maabb);

	// write sub meshes
	for (int i = 0; i < pMesh->getNumSubMeshes(); ++i)
	{
		DiSubMesh* sub = model->CreateSubMesh();

		LogManager::getSingleton().logMessage("Writing submesh...");
		WriteSubMesh(sub, pMesh->getSubMesh(i));
		LogManager::getSingleton().logMessage("Submesh exported.");
	}

	return model;
}

void ModelConverter::WriteSubMesh( DiSubMesh* subMod, const Ogre::SubMesh* s )
{
	switch(s->operationType)
	{
	case RenderOperation::OT_LINE_LIST:
		subMod->SetPrimitiveType(PT_LINELIST);
		break;
	case RenderOperation::OT_LINE_STRIP:
		subMod->SetPrimitiveType(PT_LINESTRIP);
		break;
	case RenderOperation::OT_POINT_LIST:
		subMod->SetPrimitiveType(PT_POINTLIST);
		break;
	case RenderOperation::OT_TRIANGLE_FAN:
		subMod->SetPrimitiveType(PT_TRIANGLEFAN);
		break;
	case RenderOperation::OT_TRIANGLE_LIST:
		subMod->SetPrimitiveType(PT_TRIANGLELIST);
		break;
	case RenderOperation::OT_TRIANGLE_STRIP:
		subMod->SetPrimitiveType(PT_TRIANGLESTRIP);
		break;
	}

	VertexData* vertexData = nullptr;
	if (mMesh->sharedVertexData)
	{
		vertexData = mMesh->sharedVertexData;
	}
	else
	{
		vertexData = s->vertexData;
	}

	int numFaces = 0;
	switch(s->operationType)
	{
	case RenderOperation::OT_TRIANGLE_LIST:
		// triangle list
		numFaces = s->indexData->indexCount / 3;

		break;
	case RenderOperation::OT_LINE_LIST:
		numFaces = s->indexData->indexCount / 2;

		break;
	case RenderOperation::OT_TRIANGLE_FAN:
	case RenderOperation::OT_TRIANGLE_STRIP:
		// triangle fan or triangle strip
		numFaces = s->indexData->indexCount - 2;

		break;
	default:
		OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS, 
			"Unsupported render operation type", 
			__FUNCTION__);
	}

	subMod->SetPrimitiveCount(numFaces);
	subMod->SetVerticeNum(vertexData->vertexCount);

	// material name
	DiString matName;
	matName.Format("%s_%d.mtl",subMod->GetParentMesh()->GetName().c_str(),subMod->GetIndex());
	subMod->SetMaterialName(matName);

	bool use32BitIndexes = (!s->indexData->indexBuffer.isNull() && 
		s->indexData->indexBuffer->getType() == HardwareIndexBuffer::IT_32BIT);

	// Write each face in turn
	unsigned int* pInt = 0;
	unsigned short* pShort = 0;
	HardwareIndexBufferSharedPtr ibuf = s->indexData->indexBuffer;

	// index data
	if (use32BitIndexes)
	{
		pInt = static_cast<unsigned int*>(
			ibuf->lock(HardwareBuffer::HBL_READ_ONLY)); 
	}
	else
	{
		pShort = static_cast<unsigned short*>(
			ibuf->lock(HardwareBuffer::HBL_READ_ONLY)); 
	}

	void* indata = subMod->CreateIndexData(s->indexData->indexCount,use32BitIndexes?TRUE:FALSE);
	if (use32BitIndexes)
	{
		memcpy(indata,pInt,sizeof(unsigned int)*s->indexData->indexCount);
	}
	else
	{
		memcpy(indata,pShort,sizeof(unsigned short)*s->indexData->indexCount);
	}

	ibuf->unlock();

	// vertex declaration
	VertexDeclaration* decl = vertexData->vertexDeclaration;
	VertexBufferBinding* bind = vertexData->vertexBufferBinding;

	VertexBufferBinding::VertexBufferBindingMap::const_iterator b, bend;
	bend = bind->getBindings().end();
	// iterate over buffers
	int bindCount = 0;
	for(b = bind->getBindings().begin(); b != bend; ++b,++bindCount)
	{
		const HardwareVertexBufferSharedPtr vbuf = b->second;
		unsigned short bufferIdx = b->first;
		// get all the elements that relate to this buffer			
		VertexDeclaration::VertexElementList elems = decl->findElementsBySource(bufferIdx);
		VertexDeclaration::VertexElementList::iterator i, iend;
		iend = elems.end();

		unsigned short nuDemiureCoords = 0;
		for (i = elems.begin(); i != iend; ++i)
		{
			VertexElement& elem = *i;

			DiVertexType type = ConverteVertType(elem.getType());
            DiVertexUsage usage;
			bool texcoord = false;
			switch(elem.getSemantic())
			{
			case VES_POSITION:
				usage = VERT_USAGE_POSITION;
				break;
			case VES_NORMAL:
                usage = VERT_USAGE_NORMAL;
				break;
			case VES_TANGENT:
                usage = VERT_USAGE_TANGENT;
				break;
			case VES_BINORMAL:
                usage = VERT_USAGE_BINORMAL;
				break;
			case VES_DIFFUSE:
			case VES_SPECULAR:
                usage = VERT_USAGE_COLOR;
				break;
			case VES_TEXTURE_COORDINATES:
                usage = VERT_USAGE_TEXCOORD;
				++nuDemiureCoords;
				texcoord = true;
				break;
			default:
				DI_ERROR("Unsupported semantic");
			}

			subMod->GetVertexElements().AddElement(bindCount,type,usage,texcoord?nuDemiureCoords-1:0);
		}

		int stride = subMod->GetVertexElements().GetStreamElementsSize(bindCount);
		void* vertData = subMod->CreateSourceData(bindCount,vertexData->vertexCount,stride);

		unsigned char* pVert = static_cast<unsigned char*>(
			vbuf->lock(HardwareBuffer::HBL_READ_ONLY));
		memcpy(vertData,pVert,vertexData->vertexCount*stride);
		vbuf->unlock();
	}

	// vertex weight
	if (mMesh->hasSkeleton())
	{
		LogManager::getSingleton().logMessage("Exporting dedicated geometry bone assignments...");

		if(const_cast<SubMesh*>(s)->getBoneAssignments().empty())
		{
			SubMesh::BoneAssignmentIterator bi = mMesh->getBoneAssignmentIterator();
			while (bi.hasMoreElements())
			{
				const VertexBoneAssignment& assign = bi.getNext();
				if (assign.weight > 0.0f)
				{
					subMod->AddWeight(assign.vertexIndex,assign.boneIndex,assign.weight);
				}
			}
		}
		else
		{
			SubMesh::BoneAssignmentIterator bi = (const_cast<SubMesh*>(s))->getBoneAssignmentIterator();
			while (bi.hasMoreElements())
			{
				const VertexBoneAssignment& assign = bi.getNext();
				if (assign.weight > 0.0f)
				{
					subMod->AddWeight(assign.vertexIndex,assign.boneIndex,assign.weight);
				}
			}
		}
		
		subMod->RationaliseBoneWeights();
	}
}

void ModelConverter::ExportSkeleton( const Ogre::Skeleton* pSkeleton, const Ogre::String& filename )
{
	LogManager::getSingleton().logMessage("Populating DOM...");

	// Write main skeleton data
	LogManager::getSingleton().logMessage("Exporting bones..");

	DiMotionPtr mt = Demi::DiAssetManager::GetInstancePtr(
		)->CreateOrReplaceAsset<DiMotion>(pSkeleton->getName().c_str());
	DiSkeleton* skeleton = mt->CreateSkeleton();
	
	// save bones
	WriteSkeleton(pSkeleton,skeleton);

	LogManager::getSingleton().logMessage("Bones exported.");

	unsigned short numAnims = pSkeleton->getNumAnimations();
	String msg = "Exporting animations, count=" + StringConverter::toString(numAnims);
	LogManager::getSingleton().logMessage(msg);

	// save animations
	for (unsigned short i = 0; i < numAnims; ++i)
	{
		Animation* pAnim = pSkeleton->getAnimation(i);
		msg = "Exporting animation: " + pAnim->getName();
		LogManager::getSingleton().logMessage(msg);

		Demi::DiAnimation* anim = mt->CreateAnimation(
			pAnim->getName().c_str(), pAnim->getLength());

		// save tracks
		Animation::NodeTrackIterator trackIt = pAnim->getNodeTrackIterator();
		size_t count = 0;
		while (trackIt.hasMoreElements())
		{
			NodeAnimationTrack* track = trackIt.getNext();

			Bone* bone = (Bone*)track->getAssociatedNode();
			Demi::DiNodeClip* clip = anim->CreateNodeClip(count++,skeleton->GetBone(bone->getHandle()));
			
			// save key frames
			for (unsigned short i = 0; i < track->getNumKeyFrames(); ++i)
			{
				TransformKeyFrame* kf = track->getNodeKeyFrame(i);

				Demi::DiTransformKeyFrame* pKeyframe = clip->CreateNodeKeyFrame(kf->getTime());

				pKeyframe->SetTranslate(Demi::DiVec3(kf->getTranslate().x,
										kf->getTranslate().y,kf->getTranslate().z));
				pKeyframe->SetRotation(Demi::DiQuat(kf->getRotation().w,
										kf->getRotation().x,kf->getRotation().y,kf->getRotation().z));
				pKeyframe->SetScale(Demi::DiVec3(kf->getScale().x,
										kf->getScale().y,kf->getScale().z));
			}
		}

		LogManager::getSingleton().logMessage("Animation exported.");
	}

	if (mt)
	{
		// save to file
		DiMotionSerializer ms;
		ms.ExportMotion(mt,filename);
	}
}

void ModelConverter::WriteSkeleton( const Ogre::Skeleton* pSkel, DiSkeleton* skeleton )
{
	SkeletonBlendMode blendMode = pSkel->getBlendMode() == Ogre::ANIMBLEND_CUMULATIVE?
		Demi::ANIMBLEND_CUMULATIVE:Demi::ANIMBLEND_AVERAGE;

	skeleton->SetBlendMode(blendMode);

	unsigned short numBones = pSkel->getNumBones();
	if (numBones > MAX_BONE_NUM)
	{
		MessageBox(NULL,_T("Too many bones. sorry."),_T("Too many bones. sorry."),MB_ICONERROR | MB_OK );
		return;
	}

	// save transformations
	for (unsigned short i = 0; i < numBones; ++i)
	{
		LogManager::getSingleton().logMessage("   Exporting Bone number " + StringConverter::toString(i));
		Bone* pBone = pSkel->getBone(i);
		
		Demi::DiBone* pMtBone = skeleton->CreateBone(
			pBone->getName().c_str(), pBone->getHandle());

		Vector3 pos,scale;
		Quaternion rot;
		pos   = pBone->getPosition();
		scale = pBone->getScale();
		rot   = pBone->getOrientation();

		pMtBone->SetPosition(pos.x,pos.y,pos.z);

		Demi::DiQuat orient(rot.w,rot.x,rot.y,rot.z);
		pMtBone->SetOrientation(orient);

		pMtBone->SetScale(scale.x, scale.y, scale.z);
	}

	// save parents
	for (unsigned short i = 0; i < numBones; ++i)
	{
		Bone* pBone = pSkel->getBone(i);
		String name = pBone->getName() ;

		if ((pBone->getParent())!=NULL) // root bone
		{
			Bone* pParent = (Bone*)pBone->getParent();
			unsigned short parentID = pParent->getHandle();

			Demi::DiBone* pMtChild = skeleton->GetBone(i);
			Demi::DiBone* pMtParent = skeleton->GetBone(parentID);
			pMtParent->AddChild(pMtChild);
		}
	}
}

void ModelConverter::WriteSimpleMaterial( int boneWeightsNum, const DiString& filename )
{
	DiMaterialPtr mat = Demi::DiAssetManager::GetInstancePtr(
		)->CreateOrReplaceAsset<DiMaterial>("_tempSimple");

	if (boneWeightsNum == 0)
	{
		mat->LoadVertexShader(DiMaterialDefine::STATIC_MESH_SHADER_NAME);
	}
	else if (boneWeightsNum == 1)
	{
        mat->LoadVertexShader(DiMaterialDefine::SKINING_1_WEIGHT_SHADER);
	}
	else if (boneWeightsNum == 2)
	{
        mat->LoadVertexShader(DiMaterialDefine::SKINING_2_WEIGHTS_SHADER);
	}
	else
	{
        mat->LoadVertexShader(DiMaterialDefine::SKINING_4_WEIGHTS_SHADER);
	}

    mat->LoadPixelShader(DiMaterialDefine::DIFFUSE_PIXEL_SHADER);

	Demi::DiShaderParameter* materialIns = mat->GetShaderParameter();
	materialIns->WriteTexture2D("diffuseTexture","_default.dds");

	DiMaterialSerializer mas;
	mas.SaveMaterial(filename,mat.get());
}
