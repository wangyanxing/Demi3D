
#include "GfxPch.h"
#include "MotionSerial.h"
#include "MotionFormat.h"
#include "Skeleton.h"
#include "AttachSet.h"
#include "Animation.h"
#include "AnimationClip.h"
#include "Bone.h"
#include "AttachNode.h"
#include "KeyFrame.h"
#include "AssetManager.h"

namespace Demi
{
    DiMotionSerializer::DiMotionSerializer()
    {

    }

    DiMotionSerializer::~DiMotionSerializer()
    {

    }

    void DiMotionSerializer::ExportMotion( const DiMotionPtr pMotion, DiDataStreamPtr stream )
    {
        char sign[4];
        sign[0] = 'M';
        sign[1] = '3';
        sign[2] = 'G';
        sign[3] = 'M';
        stream->Write(sign,4);

        uint16 version = MOTION_SERIAL_VERSION_NUM - 1;
        stream->Write(&version,sizeof(uint16));

        DiMotionSerializerImpl* impl = GetImplemention((DiMotionSerialVersion)version);
        if (impl)
        {
            impl->ExportMotion(pMotion.get(),stream);
        }

        DI_DELETE impl;
    }

    void DiMotionSerializer::ExportMotion( const DiMotionPtr pMotion, const DiString& filename )
    {
        FILE* fp = fopen(filename.c_str(),"wb");
        DiDataStreamPtr ds(DI_NEW DiFileHandleDataStream(fp,DiDataStream::WRITE));
        ExportMotion(pMotion,ds);
    }

    void DiMotionSerializer::ExportMotion( const DiString& motionName, const DiString& filename )
    {
        DiMotionPtr md = DiAssetManager::GetInstance().GetAsset<DiMotion>(motionName);
        ExportMotion(md,filename);
    }

    void DiMotionSerializer::ImportMotion( DiDataStreamPtr& stream, DiMotion* pDest )
    {
        char sign[4];
        stream->Read(sign,4);

        if (sign[0] == 'M' &&
            sign[1] == '3' &&
            sign[2] == 'G' &&
            sign[3] == 'M')
        {
            uint16 version;
            stream->Read(&version,sizeof(uint16));

            if (version >= MOTION_SERIAL_VERSION_NUM)
            {
                DI_ERROR("Invalid version number!");
                return;
            }

            DiMotionSerializerImpl* impl = GetImplemention((DiMotionSerialVersion)version);
            if (impl)
            {
                impl->ImportMotion(stream,pDest);
            }

            DI_DELETE impl;
            return;
        }

        DI_ERROR("Invalid head format!");
        return;
    }

    DiMotionSerializerImpl* DiMotionSerializer::GetImplemention( DiMotionSerialVersion ver )
    {
        if (ver == MOTION_SERIAL_VERSION_0)
        {
            return DI_NEW DiMotionSerializerImpl();
        }
        else 
        {
            return NULL;
        }
    }

    //////////////////////////////////////////////////////////////////////////

    DiMotionSerializerImpl::DiMotionSerializerImpl()
    {

    }

    DiMotionSerializerImpl::~DiMotionSerializerImpl()
    {

    }

    void DiMotionSerializerImpl::ExportMotion( const DiMotion* pMotion, DiDataStreamPtr stream )
    {
        mStream = stream;

        WriteFileHeader();

        if (pMotion->GetSkeleton())
        {
            WriteSkeleton(pMotion->GetSkeleton());
        }

        if (pMotion->GetAttachSet())
        {
            WriteAttachSet(pMotion->GetAttachSet());
        }

        DiMotion::ConstAnimationListIterator i = pMotion->GetAnimations();
        while (i.HasMoreElements())
        {
            WriteAnimation(i.GetNext());
        }
    }

    void DiMotionSerializerImpl::ImportMotion( DiDataStreamPtr& stream, DiMotion* pDest )
    {
        ReadFileHeader(stream);

        if (!stream->Eof())
        {
            uint16 streamID = ReadChunk(stream);
            while(!stream->Eof() &&
                (streamID == DI_SKELETON ||
                streamID == DI_ATTACHSET ||
                streamID == DI_POSE ||
                streamID == DI_ANIMATION))
            {
                switch(streamID)
                {
                case DI_SKELETON:
                    ReadSkeleton(stream,pDest);
                    break;

                case DI_ANIMATION:
                    ReadAnimations(stream, pDest);
                    break;

                case DI_POSE:
                    ReadPoses(stream,pDest);
                    break;

                case DI_ATTACHSET:
                    ReadAttachSet(stream,pDest);
                    break;
                }

                if (!stream->Eof())
                    streamID = ReadChunk(stream);

            }
            if (!stream->Eof())
                stream->Skip(-MSTREAM_OVERHEAD_SIZE);
        }
    }

    void DiMotionSerializerImpl::ReadFileHeader( DiDataStreamPtr& )
    {

    }

    void DiMotionSerializerImpl::WriteFileHeader( void )
    {

    }

    void DiMotionSerializerImpl::WriteSkeleton( const DiSkeleton* pSkeleton )
    {
        WriteChunkHeader(DI_SKELETON,sizeof(uint16) + MSTREAM_OVERHEAD_SIZE);

        uint16 blendmode = (uint16)pSkeleton->GetBlendMode();
        WriteShorts(&blendmode,1);

        if (pSkeleton->GetNumBones() > 0)
        {
            WriteBones(pSkeleton);
        }

        size_t numBones = pSkeleton->GetNumBones();
        for (size_t i = 0; i < numBones; ++i)
        {
            const DiBone* pBone = pSkeleton->GetBone(i);
            unsigned short handle = (uint16)pBone->GetHandle();
            DiBone* pParent = (DiBone*)pBone->GetParent(); 
            if (pParent != NULL) 
            {
                WriteBoneParent((uint16)handle, (uint16)pParent->GetHandle());             
            }
        }
    }

    void DiMotionSerializerImpl::WriteBones( const DiSkeleton* pSkeleton )
    {
        WriteChunkHeader(DI_BONES,CalcBonesSize(pSkeleton));

        uint16 numBones = (uint16)pSkeleton->GetNumBones();
        WriteShorts(&numBones,1);

        for (size_t i = 0; i < numBones; ++i)
        {
            const DiBone* pBone = pSkeleton->GetBone(i);

            // name
            WriteString(pBone->GetName());

            // handle
            uint16 handle = (uint16)pBone->GetHandle();
            WriteShorts(&handle,1);

            // position
            WriteObject(pBone->GetPosition());

            // orientation
            WriteObject(pBone->GetOrientation());

            // ÊÇ·ñÓÐscale
            bool hasscale = pBone->GetScale() != DiVec3::UNIT_SCALE;
            WriteBools(&hasscale,1);

            if (hasscale)
            {
                // scale
                WriteObject(pBone->GetScale());
            }
        }
    }

    void DiMotionSerializerImpl::WriteBoneParent( uint16 b0, uint16 bparent )
    {
        WriteChunkHeader(DI_BONES_PARENT,sizeof(uint16)*2 + MSTREAM_OVERHEAD_SIZE);

        WriteShorts(&b0, 1);

        WriteShorts(&bparent, 1);
    }

    size_t DiMotionSerializerImpl::CalcBonesSize(const DiSkeleton* pSkeleton)
    {
        size_t size = MSTREAM_OVERHEAD_SIZE;

        // ¹Ç÷ÀÊý
        size += sizeof(uint16);

        size_t numBones = pSkeleton->GetNumBones();
        for (size_t i = 0; i < numBones; ++i)
        {
            const DiBone* pBone = pSkeleton->GetBone(i);

            // name
            size += (pBone->GetName().length() + 1);

            // handle
            size += sizeof(uint16);

            // transformation
            size += sizeof(float) * 3;
            size += sizeof(float) * 4;
            size += sizeof(bool);

            if (pBone->GetScale() != DiVec3::UNIT_SCALE)
            {
                size += sizeof(float) * 3;
            }
        }

        return size;
    }

    void DiMotionSerializerImpl::WriteAnimation( const DiAnimation* pkAnimation )
    {
        WriteChunkHeader(DI_ANIMATION,CalcAnimationSize(pkAnimation));

        // name
        WriteString(pkAnimation->GetName());

        // length
        float len = pkAnimation->GetLength();
        WriteFloats(&len, 1);

        auto trackIt = pkAnimation->GetNodeClipsIterator();
        while(trackIt.HasMoreElements())
        {
            WriteNodeClip(trackIt.GetNext());
        }

        auto attachIt = pkAnimation->GetAttachClipsIterator();
        while(attachIt.HasMoreElements())
        {
            DiNodeClip * pAttach = attachIt.GetNext();
            WriteAttachClip(pAttach);
        }

    }

    size_t DiMotionSerializerImpl::CalcAnimationSize( const DiAnimation* pkAnim )
    {
        size_t size = MSTREAM_OVERHEAD_SIZE;

        // name
        size += pkAnim->GetName().length() + 1;

        // length
        size += sizeof(float);

        return size;
    }

    size_t DiMotionSerializerImpl::CalcNodeClipSize( const DiNodeClip* pClip )
    {
        size_t size = MSTREAM_OVERHEAD_SIZE;

        // handle
        size += sizeof(uint16);

        size += sizeof(uint16);

        for (unsigned short i = 0; i < pClip->GetNumFrames(); ++i)
        {
            size += sizeof(float);
            size += sizeof(float) * 3;
            size += sizeof(float) * 4;
            size += sizeof(bool);
            if (pClip->GetNodeKeyFrame(i)->GetScale() != DiVec3::UNIT_SCALE)
            {
                size += sizeof(float) * 3;
            }
        }

        return size;
    }

    void DiMotionSerializerImpl::WriteNodeClip( const DiNodeClip* pClip )
    {
        DiBone* bone = (DiBone*)pClip->GetTargetNode();
        if (!bone)
        {
            return;
        }

        WriteChunkHeader(DI_CLIPS_NODE,CalcNodeClipSize(pClip));

        uint16 boneid = (uint16)bone->GetHandle();
        WriteShorts(&boneid, 1);

        uint16 keyFrameNum = (uint16) pClip->GetNumFrames();
        WriteShorts(&keyFrameNum,1);
        
        for (uint16 i = 0; i < keyFrameNum; ++i)
        {
            DiTransformKeyFrame* key = pClip->GetNodeKeyFrame(i);

            float time = key->GetTime();
            WriteFloats(&time, 1);

            WriteObject(key->GetTranslate());

            WriteObject(key->GetRotation());
            
            bool hasScale = key->GetScale() != DiVec3::UNIT_SCALE;
            WriteBools(&hasScale,1);

            if (hasScale)
            {
                WriteObject(key->GetScale());
            }
        }
    }

    void DiMotionSerializerImpl::ReadSkeleton( DiDataStreamPtr& stream,DiMotion* motion )
    {
        DiSkeleton* skel = motion->CreateSkeleton();
    
        uint16 blendmode;
        ReadShorts(stream,&blendmode,1);
        skel->SetBlendMode(SkeletonBlendMode(blendmode));

        if (!stream->Eof())
        {
            uint16 streamID = ReadChunk(stream);
            while(!stream->Eof() &&
                (streamID == DI_BONES ||
                streamID == DI_BONES_PARENT))
            {
                switch(streamID)
                {
                case DI_BONES:
                    ReadBones(stream,skel);
                    break;

                case DI_BONES_PARENT:
                    ReadBonesParents(stream,skel);
                    break;
                }

                if (!stream->Eof())
                {
                    streamID = ReadChunk(stream);
                }

            }
            if (!stream->Eof())
            {
                stream->Skip(-MSTREAM_OVERHEAD_SIZE);
            }
        }
    }

    void DiMotionSerializerImpl::ReadBones( DiDataStreamPtr& stream,DiSkeleton* skeleton )
    {
        uint16 numBones;

        ReadShorts(stream,&numBones,1);

        for (size_t i = 0; i < numBones; ++i)
        {
            // name
            DiString name = ReadString(stream);

            // handle
            uint16 handle;
            ReadShorts(stream,&handle,1);

            // position
            DiVec3 position;
            ReadObject(stream,position);

            // orientation
            DiQuat quat;
            ReadObject(stream,quat);

            // scale
            bool hasscale;
            ReadBools(stream,&hasscale,1);

            DiVec3 scale = DiVec3::UNIT_SCALE;
            if (hasscale)
            {
                // scale
                ReadObject(stream,scale);
            }

            DiBone* bone = skeleton->CreateBone(name,handle);
            bone->SetPosition(position);
            bone->SetOrientation(quat);
            if (hasscale)
            {
                bone->SetScale(scale);
            }
        }
    }

    void DiMotionSerializerImpl::ReadBonesParents( DiDataStreamPtr& stream,DiSkeleton* skeleton )
    {
        uint16 handleSon = 0;
        ReadShorts(stream,&handleSon, 1);

        uint16 handleParent = 0;
        ReadShorts(stream,&handleParent, 1);

        DiBone* son = skeleton->GetBone(handleSon);
        DiBone* parent = skeleton->GetBone(handleParent);
        parent->AddChild(son);
    }

    void DiMotionSerializerImpl::ReadAnimations( DiDataStreamPtr& stream, DiMotion* motion )
    {
        // name
        DiString name = ReadString(stream);

        // length
        float len = 0;
        ReadFloats(stream,&len, 1);

        DiAnimation* anim = motion->CreateAnimation(name,len);

        if (!stream->Eof())
        {
            uint16 streamID = ReadChunk(stream);
            while(!stream->Eof() &&
                (streamID == DI_CLIPS_NODE ||
                streamID == DI_CLIPS_ATTACH ||
                streamID == DI_CLIPS_POSE))
            {
                switch(streamID)
                {
                case DI_CLIPS_NODE:
                    ReadNodeClips(stream,anim);
                    break;

                case DI_CLIPS_POSE:
                    ReadPoseClips(stream,anim);
                    break;

                case DI_CLIPS_ATTACH:
                    ReadAttachClips(stream,anim);
                    break;
                }

                if (!stream->Eof())
                {
                    streamID = ReadChunk(stream);
                }

            }
            if (!stream->Eof())
            {
                stream->Skip(-MSTREAM_OVERHEAD_SIZE);
            }
        }
    }

    void DiMotionSerializerImpl::ReadNodeClips( DiDataStreamPtr& stream, DiAnimation* motion)
    {
        uint16 boneid = 0;
        ReadShorts(stream,&boneid, 1);

        DiNodeClip* nodeClip = motion->CreateNodeClip(boneid);

        uint16 keyFrameNum = 0;
        ReadShorts(stream,&keyFrameNum,1);

        for (uint16 i = 0; i < keyFrameNum; ++i)
        {
            float time = 0;
            ReadFloats(stream,&time, 1);

            DiVec3 pos;
            ReadObject(stream,pos);

            DiQuat quat;
            ReadObject(stream,quat);

            bool hasScale = false;
            ReadBools(stream,&hasScale,1);

            DiVec3 scale = DiVec3::UNIT_SCALE;
            if (hasScale)
            {
                ReadObject(stream,scale);
            }

            DiTransformKeyFrame* key = nodeClip->CreateNodeKeyFrame(time);
            key->SetTranslate(pos);
            key->SetRotation(quat);
            key->SetScale(scale);
        }
    }

    size_t DiMotionSerializerImpl::CalcAttachSize( const DiAttachSet* pkAttachSet )
    {
        size_t size = MSTREAM_OVERHEAD_SIZE;

        size += sizeof(uint16);

        DiAttachSet::AttachNodeMap::const_iterator itAttach = pkAttachSet->GetAttaches().begin();
        
        for ( ; itAttach != pkAttachSet->GetAttaches().end() ; ++ itAttach )
        {
            const DiAttachNode * pkAttNode = itAttach->second;

            // name
            size += (pkAttNode->GetName().length() + 1);

            size += sizeof(float) * 3;

            size += sizeof(float) * 4;

            size += sizeof(bool);

            if (pkAttNode->GetScale() != DiVec3::UNIT_SCALE)
            {
                size += sizeof(float) * 3;
            }
        }

        return size;
    }

    void DiMotionSerializerImpl::ReadAttachSet( DiDataStreamPtr& stream,DiMotion* motion )
    {
        DiAttachSet * attachset = motion->CreateAttachSet();

        if (!stream->Eof())
        {
            uint16 streamID = ReadChunk(stream);
            while(!stream->Eof() &&
                (streamID == DI_ATTACHS ||
                streamID == DI_ATTACHS_PARENT))
            {
                switch(streamID)
                {
                case DI_ATTACHS:
                    ReadAttachNodes(stream,attachset);
                    break;

                case DI_ATTACHS_PARENT:
                    ReadAttachParents(stream,attachset,motion->GetSkeleton());
                    break;
                }

                if (!stream->Eof())
                {
                    streamID = ReadChunk(stream);
                }

            }
            if (!stream->Eof())
            {
                stream->Skip(- MSTREAM_OVERHEAD_SIZE);
            }
        }
    }

    void DiMotionSerializerImpl::ReadAttachNodes( DiDataStreamPtr& stream,DiAttachSet* attachset )
    {
        uint16 numAttaches;

        ReadShorts(stream,&numAttaches,1);

        for (size_t i = 0; i < numAttaches; ++i)
        {
            DiString name = ReadString(stream);

            DiVec3 position;
            ReadObject(stream,position);

            DiQuat quat;
            ReadObject(stream,quat);

            bool hasscale;
            ReadBools(stream,&hasscale,1);

            DiVec3 scale = DiVec3::UNIT_SCALE;
            if (hasscale)
            {
                ReadObject(stream,scale);
            }

            DiAttachNode * pkAttachNode = attachset->CreateAttachNode(name);
            pkAttachNode->SetPosition(position);
            pkAttachNode->SetOrientation(quat);
            
            if (hasscale)
            {
                pkAttachNode->SetScale(scale);
            }
        }
    }

    void DiMotionSerializerImpl::ReadAttachParents( DiDataStreamPtr& stream,DiAttachSet* attachset,DiSkeleton* skeleton )
    {
        DiString strChildName = ReadString(stream);
        DiString strParentName = ReadString(stream);

        DiNode* son = attachset->GetAttachNode(strChildName);
        DiNode* parent = NULL;
        parent = attachset->GetAttachNode(strParentName);

        if(!parent && skeleton)
        {
            if(skeleton->HasBone(strParentName))
            {
                parent = skeleton->GetBone(strParentName);
            }
        }

        if(parent)
        {
            parent->AddChild(son);
        }
        else
        {
            DI_WARNING("No parent : %s",strParentName.c_str());
        }
    }

    void DiMotionSerializerImpl::ReadAttachClips( DiDataStreamPtr& stream, DiAnimation* motion )
    {
        DiString strName = ReadString(stream);

        DiNodeClip* nodeClip = motion->CreateAttachClip(strName);

        uint16 keyFrameNum = 0;
        ReadShorts(stream,&keyFrameNum,1);

        for (uint16 i = 0; i < keyFrameNum; ++i)
        {
            float time = 0;
            ReadFloats(stream,&time, 1);

            DiVec3 pos;
            ReadObject(stream,pos);

            DiQuat quat;
            ReadObject(stream,quat);

            bool hasScale = false;
            ReadBools(stream,&hasScale,1);

            DiVec3 scale = DiVec3::UNIT_SCALE;
            if (hasScale)
            {
                ReadObject(stream,scale);
            }

            DiTransformKeyFrame* key = nodeClip->CreateNodeKeyFrame(time);
            key->SetTranslate(pos);
            key->SetRotation(quat);
            key->SetScale(scale);
        }
    }

    void DiMotionSerializerImpl::WriteAttachSet( const DiAttachSet* pkAttachset )
    {
        WriteChunkHeader(DI_ATTACHSET,sizeof(uint16) + MSTREAM_OVERHEAD_SIZE);

        if (pkAttachset->GetNumAttaches() > 0)
        {
            WriteAttachNode(pkAttachset);
        }

        DiAttachSet::AttachNodeMap::const_iterator itAttach = pkAttachset->GetAttaches().begin(); 
        for (; itAttach != pkAttachset->GetAttaches().end() ; ++ itAttach )
        {
            const DiAttachNode * pkAttachNode = itAttach->second;
            
            DiString strNode = pkAttachNode->GetName();
            DiNode * pkParent = pkAttachNode->GetParent();
            if (pkParent != NULL) 
            {
                WriteAttachParents(strNode,pkParent->GetName());             
            }
        }
    }

    void DiMotionSerializerImpl::WriteAttachNode( const DiAttachSet* pkAttachset )
    {
        WriteChunkHeader(DI_ATTACHS,CalcAttachSize(pkAttachset));

        uint16 numAttaches = (uint16)pkAttachset->GetNumAttaches();
        WriteShorts(&numAttaches,1);

        DiAttachSet::AttachNodeMap::const_iterator itAttach = pkAttachset->GetAttaches().begin(); 
        for (; itAttach != pkAttachset->GetAttaches().end() ; ++ itAttach )
        {
            const DiAttachNode * pkAttachNode = itAttach->second;

            WriteString(pkAttachNode->GetName());

            WriteObject(pkAttachNode->GetPosition());

            WriteObject(pkAttachNode->GetOrientation());

            bool hasscale = pkAttachNode->GetScale() != DiVec3::UNIT_SCALE;
            WriteBools(&hasscale,1);

            if (hasscale)
            {
                WriteObject(pkAttachNode->GetScale());
            }
        }
    }

    void DiMotionSerializerImpl::WriteAttachParents( const DiString& strNode,const DiString strParent )
    {
        size_t cbNodeNameLen    = strNode.length() + 1;
        size_t cbParentNameLen    = strParent.length() + 1;

        WriteChunkHeader(DI_ATTACHS_PARENT,cbNodeNameLen + cbParentNameLen + MSTREAM_OVERHEAD_SIZE);

        WriteString(strNode);

        WriteString(strParent);
    }

    size_t DiMotionSerializerImpl::CalcAttachClipSize( const DiNodeClip* pClip )
    {
        size_t size = MSTREAM_OVERHEAD_SIZE;

        size += (pClip->GetTargetNode()->GetName().length() + 1);

        size += sizeof(uint16);

        for (unsigned short i = 0; i < pClip->GetNumFrames(); ++i)
        {
            size += sizeof(float);
            size += sizeof(float) * 3;
            size += sizeof(float) * 4;
            size += sizeof(bool);
            if (pClip->GetNodeKeyFrame(i)->GetScale() != DiVec3::UNIT_SCALE)
            {
                size += sizeof(float) * 3;
            }
        }

        return size;
    }

    void DiMotionSerializerImpl::WriteAttachClip( const DiNodeClip* clip )
    {
        DiAttachNode * attach = (DiAttachNode *)clip->GetTargetNode();
        if (!attach)
        {
            DI_WARNING("No target node!");
            return;
        }

        WriteChunkHeader(DI_CLIPS_ATTACH,CalcAttachClipSize(clip));

        WriteString(attach->GetName());

        uint16 keyFrameNum = (uint16) clip->GetNumFrames();
        WriteShorts(&keyFrameNum,1);

        for (uint16 i = 0; i < keyFrameNum; ++i)
        {
            DiTransformKeyFrame* key = clip->GetNodeKeyFrame(i);

            float time = key->GetTime();
            WriteFloats(&time, 1);

            WriteObject(key->GetTranslate());

            WriteObject(key->GetRotation());

            bool hasScale = key->GetScale() != DiVec3::UNIT_SCALE;
            WriteBools(&hasScale,1);

            if (hasScale)
            {
                WriteObject(key->GetScale());
            }
        }
    }

    void DiMotionSerializerImpl::ReadPoses( DiDataStreamPtr& /*stream*/, DiMotion* /*motion*/ )
    {

    }

    void DiMotionSerializerImpl::ReadPoseClips( DiDataStreamPtr& , DiAnimation*  )
    {

    }



}