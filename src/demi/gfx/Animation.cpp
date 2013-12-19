
#include "Animation.h"
#include "Skeleton.h"
#include "Bone.h"
#include "AttachSet.h"
#include "AttachNode.h"
#include "Algorithm.h"

namespace Demi
{

    DiAnimation::DiAnimation( const DiString& name, float length )
        :mName(name),
        mLength(length),
        mInterMode(DEFAULT_INTER_MODE),
        mRotationInterMode(DEFAULT_ROTATION_INTER_MODE),
        mKeyFrameTimesDirty(false)
    {

    }

    DiAnimation::~DiAnimation()
    {
        DestroyAllClips();
    }

    DiNodeClip* DiAnimation::GetNodeClip( uint32 handle ) const
    {
        NodeClips::const_iterator i = mNodeClips.find(handle);

        if (i == mNodeClips.end())
        {
            DI_ERROR("不存在handle值=%d的节点动画组",handle);
            return NULL;
        }

        return i->second;
    }

    void DiAnimation::SetRotationInterMode( RotationInterMode im )
    {
        mRotationInterMode = im;
    }

    DiNodeClip* DiAnimation::CreateNodeClip( uint32 handle )
    {
        if (HasNodeClip(handle))
        {
            DI_ERROR("已存在同样handle值的节点动画组");
            return NULL;
        }

        DiNodeClip* ret = DI_NEW DiNodeClip(this, handle);
        mNodeClips[handle] = ret;
        return ret;
    }

    DiNodeClip* DiAnimation::CreateNodeClip( uint32 handle,DiNode* node )
    {
        DiNodeClip* ret = CreateNodeClip(handle);
        ret->SetTargetNode(node);
        return ret;
    }

    bool DiAnimation::HasNodeClip( uint32 handle ) const
    {
        return (mNodeClips.find(handle) != mNodeClips.end());
    }

    void DiAnimation::DestroyNodeClip( uint32 handle )
    {
        NodeClips::iterator i = mNodeClips.find(handle);

        if (i != mNodeClips.end())
        {
            DI_DELETE i->second;
            mNodeClips.erase(i);
            KeyFrameListChanged();
        }
    }

    void DiAnimation::DestroyAllNodeClips()
    {
        NodeClips::iterator i;
        for (i = mNodeClips.begin(); i != mNodeClips.end(); ++i)
        {
            DI_DELETE i->second;
        }
        mNodeClips.clear();
        KeyFrameListChanged();
    }

     DiNodeClip* DiAnimation::CreateAttachClip( DiString strAttachName )
     {
         if (HasAttachClip(strAttachName))
         {
             DI_ERROR("已存在同名的绑点动画组");
             return NULL;
         }
 
         DiNodeClip* ret = DI_NEW DiNodeClip(this, strAttachName.ToHash());
         mAttachClips[strAttachName] = ret;
 
         return ret;
     }
 
     DiNodeClip* DiAnimation::CreateAttachClip( DiString strAttachName,DiNode * pkNode )
     {
         DiNodeClip* ret = CreateAttachClip(strAttachName);
 
         ret->SetTargetNode(pkNode);
         
         return ret;
     }
 
     bool DiAnimation::HasAttachClip( DiString strAttachName )
     {
         return (mAttachClips.find(strAttachName) != mAttachClips.end());
     }
 
     void DiAnimation::DestroyAttachClip( DiString strAttachName )
     {
         AttachClips::iterator i = mAttachClips.find(strAttachName);
 
         if (i != mAttachClips.end())
         {
             DI_DELETE i->second;
             mAttachClips.erase(i);
 
             KeyFrameListChanged();
         }
     }

     void DiAnimation::DestroyAllAttachClips()
     {
         AttachClips::iterator i;
         for (i = mAttachClips.begin(); i != mAttachClips.end(); ++i)
         {
             DI_DELETE i->second;
         }
 
         mAttachClips.clear();
         
         KeyFrameListChanged();
     }

    void DiAnimation::DestroyAllClips()
    {
        DestroyAllAttachClips();
        DestroyAllNodeClips();
    }

    void DiAnimation::SetInterMode( InterMode im )
    {
        mInterMode = im;
    }

    void DiAnimation::ApplySkeleton( DiSkeleton* skel, DiTimeIndex& timeIndex, float weight, float scale )
    {
        NodeClips::iterator i;
        for (i = mNodeClips.begin(); i != mNodeClips.end(); ++i)
        {
            DiBone* b = skel->GetBone(i->first);
            i->second->ApplyToNode(b, timeIndex, weight, scale);
        }
    }

    void DiAnimation::ApplySkeleton( DiSkeleton* skel,  DiTimeIndex& timeIndex, 
        float weight, const DiClipController::BoneBlendMask* blendMask, float scale )
    {
        NodeClips::iterator i;
        for (i = mNodeClips.begin(); i != mNodeClips.end(); ++i)
        {
            DiBone* b = skel->GetBone(i->first);
            i->second->ApplyToNode(b, timeIndex, (*blendMask)[b->GetHandle()] * weight, scale);
        }
    }

    void DiAnimation::ApplyAttachSet( DiAttachSet* attachset, DiTimeIndex& timeIndex, float weight, float scale )
    {
        AttachClips::iterator i;
        for (i = mAttachClips.begin(); i != mAttachClips.end(); ++i)
        {
            DiAttachNode * an = attachset->GetAttachNode(i->first);
            i->second->ApplyToNode(an, timeIndex, weight, scale);
        }
    }

    void DiAnimation::ApplyAttachSet( DiAttachSet* attachset, DiTimeIndex& timeIndex, float weight, const DiClipController::AttachBlendMask& blendMask, float scale )
    {
        AttachClips::iterator i;
        for (i = mAttachClips.begin(); i != mAttachClips.end(); ++i)
        {
            DiAttachNode * an = attachset->GetAttachNode(i->first);

            DiClipController::AttachBlendMask::const_iterator itAttach = blendMask.find(an->GetAttachName());
            if(itAttach == blendMask.end())
                continue;

            float fBlend = itAttach->second;
            i->second->ApplyToNode(an, timeIndex, fBlend * weight, scale);
        }
    }

    Demi::DiTimeIndex DiAnimation::GetTimeIndex( float timePos ) const
    {
        if (mKeyFrameTimesDirty)
        {
            BuildKeyFrameTimeList();
        }

        float totalAnimationLength = mLength;

        if( timePos > totalAnimationLength && totalAnimationLength > 0.0f )
        {
            timePos = fmod( timePos, totalAnimationLength );
        }

        KeyFrameTimeList::iterator it =
            Demi::LowerBound(mKeyFrameTimes.begin(), mKeyFrameTimes.end(), timePos);

        return DiTimeIndex(timePos, Demi::Distance(mKeyFrameTimes.begin(), it));

//         KeyFrameTimeList::iterator it;
//         for (it = m_kKeyFrameTimes.begin(); it!=m_kKeyFrameTimes.end(); ++it)
//         {
//             if (*it >= timePos)
//             {
//                 break;
//             }
//         }
// 
//         return TimeIndex(timePos, it - m_kKeyFrameTimes.begin());
    }

    void DiAnimation::BuildKeyFrameTimeList( void ) const
    {

        mKeyFrameTimes.clear();

        NodeClips::const_iterator itNodeClip;

        for (itNodeClip = mNodeClips.begin(); itNodeClip != mNodeClips.end(); ++ itNodeClip)
        {
            itNodeClip->second->CollectKeyFrameTimes(mKeyFrameTimes);
        }
    
        for (itNodeClip = mNodeClips.begin(); itNodeClip != mNodeClips.end(); ++ itNodeClip)
        {
            itNodeClip->second->BuildKeyFrameIndexMap(mKeyFrameTimes);
        }

        AttachClips::const_iterator itAttClip;

        for (itAttClip = mAttachClips.begin(); itAttClip != mAttachClips.end(); ++ itAttClip)
        {
            itAttClip->second->CollectKeyFrameTimes(mKeyFrameTimes);
        }

        for (itAttClip = mAttachClips.begin(); itAttClip != mAttachClips.end(); ++ itAttClip)
        {
            itAttClip->second->BuildKeyFrameIndexMap(mKeyFrameTimes);
        }
        
        mKeyFrameTimesDirty = false;
    }

    DiAnimation::NodeClipsIterator DiAnimation::GetNodeClipsIterator() const
    {
        return NodeClipsIterator(mNodeClips.begin(),mNodeClips.end());
    }

    DiAnimation::AttachClipsIterator DiAnimation::GetAttachClipsIterator() const
    {
        return AttachClipsIterator(mAttachClips.begin(),mAttachClips.end());
    }

    void DiAnimation::NodeClipAttachToSkeleton( DiSkeleton* skeleton )
    {
        NodeClips::iterator it;
        NodeClips::iterator itEnd = mNodeClips.end();
        for (it = mNodeClips.begin(); it != itEnd; ++it)
        {
            DiNodeClip* nc = it->second;
            uint32 handle = it->first;
            DiBone* bone = skeleton->GetBone(handle);
            nc->SetTargetNode(bone);
        }
    }

    void DiAnimation::NodeClipAttachToAttachSet( DiAttachSet* attachset )
    {
        AttachClips::iterator it;
        AttachClips::iterator itEnd = mAttachClips.end();
        for (it = mAttachClips.begin(); it != itEnd; ++it)
        {
            DiNodeClip* nc = it->second;
            DiString strName = it->first;
            DiAttachNode* pkAttachNode = attachset->GetAttachNode(strName);
            nc->SetTargetNode(pkAttachNode);
        }
    }

    DiAnimation::RotationInterMode DiAnimation::DEFAULT_ROTATION_INTER_MODE = DiAnimation::RIM_LINEAR;
    DiAnimation::InterMode DiAnimation::DEFAULT_INTER_MODE = DiAnimation::IM_LINEAR;

}
