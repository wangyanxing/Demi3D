
/********************************************************************
    File:       MotionSerial.h
    Creator:    demiwangya
*********************************************************************/

#pragma once

#include "Serializer.h"
#include "Motion.h"

namespace Demi
{
    class DiMotionSerializerImpl;

    enum DiMotionSerialVersion
    {
        MOTION_SERIAL_VERSION_0 = 0,

        MOTION_SERIAL_VERSION_NUM
    };

    class DI_GFX_API DiMotionSerializer : public DiBase
    {
    public:

        DiMotionSerializer();

        virtual                 ~DiMotionSerializer();

    public:

        void                    ExportMotion(const DiMotionPtr pMotion, DiDataStreamPtr stream);

        void                    ExportMotion(const DiMotionPtr pMotion, const DiString& filename);

        void                    ExportMotion(const DiString& motionName, const DiString& filename);

        void                    ImportMotion(DiDataStreamPtr& stream, DiMotion* pDest);

    protected:

        DiMotionSerializerImpl*  GetImplemention(DiMotionSerialVersion ver);
    };

    class DiMotionSerializerImpl : public DiSerializer
    {
    public:

        DiMotionSerializerImpl();

        virtual                 ~DiMotionSerializerImpl();

    public:

        void                    ExportMotion(const DiMotion* pMotion, DiDataStreamPtr stream);

        void                    ImportMotion(DiDataStreamPtr& stream, DiMotion* pDest);

    protected:

        virtual void            ReadFileHeader(DiDataStreamPtr& stream);

        virtual void            ReadSkeleton(DiDataStreamPtr& stream,DiMotion* motion);
 
        virtual void            ReadAttachSet(DiDataStreamPtr& stream,DiMotion* motion);

        virtual void            ReadBones(DiDataStreamPtr& stream,DiSkeleton* skeleton);

        virtual void            ReadAttachNodes(DiDataStreamPtr& stream,DiAttachSet* attachset);

        virtual void            ReadBonesParents(DiDataStreamPtr& stream,DiSkeleton* skeleton);

        virtual void            ReadAttachParents(DiDataStreamPtr& stream,DiAttachSet* attachset,DiSkeleton* skeleton);

        virtual void            ReadAnimations(DiDataStreamPtr& stream, DiMotion* motion);

        virtual void            ReadPoses(DiDataStreamPtr& stream, DiMotion* motion);

        virtual void            ReadNodeClips(DiDataStreamPtr& stream, DiAnimation* motion);

        virtual void            ReadPoseClips(DiDataStreamPtr& stream, DiAnimation* motion);

        virtual void            ReadAttachClips(DiDataStreamPtr& stream, DiAnimation* motion);

        virtual void            WriteFileHeader(void);

        virtual void            WriteAttachSet(const DiAttachSet* pkAttachset);

        virtual void            WriteSkeleton(const DiSkeleton* pSkeleton);

        virtual void            WriteAttachNode(const DiAttachSet* pkAttachset);

        virtual void            WriteBones(const DiSkeleton* pSkeleton);

        virtual void            WriteAttachParents(const DiString& strNode,const DiString strParent);

        virtual void            WriteBoneParent(uint16 b0, uint16 b1);

        virtual void            WriteAnimation(const DiAnimation* pAnimation);

        virtual void            WriteNodeClip(const DiNodeClip* clip);

        virtual void            WriteAttachClip(const DiNodeClip* clip);

        virtual size_t          CalcBonesSize(const DiSkeleton* pSkeleton);
                                
        virtual size_t          CalcAttachSize(const DiAttachSet* pkAttachSet);

        virtual size_t          CalcAnimationSize(const DiAnimation* pAnim);

        virtual size_t          CalcNodeClipSize(const DiNodeClip* pClip);

        virtual size_t          CalcAttachClipSize(const DiNodeClip* pClip);
    };
}