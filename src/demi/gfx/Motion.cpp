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
#include "GfxPch.h"
#include "Motion.h"
#include "Skeleton.h"
#include "AttachSet.h"
#include "Animation.h"
#include "MotionSerial.h"

namespace Demi
{
    DiString DiMotion::TYPE = "Motion";

    DiMotion::DiMotion( const DiString& name )
        :DiAsset(name),
        mSkeleton(nullptr),
        mAttachSet(nullptr)
    {
    }

    DiMotion::~DiMotion()
    {
        SAFE_DELETE(mSkeleton);
        
        for (auto it = mAnimationList.begin(); it != mAnimationList.end(); ++it)
        {
            SAFE_DELETE(it->second);
        }

        mAnimationList.clear();
    }

    bool DiMotion::LoadingComplete() const
    {
        return true;
    }

    bool DiMotion::Load( DiDataStreamPtr data )
    {
        DiMotionSerializer ms;
        ms.ImportMotion(data,this);
        return true;
    }

    bool DiMotion::Load()
    {
        return true;
    }

    DiSkeleton* DiMotion::CreateSkeleton()
    {
        SAFE_DELETE(mSkeleton);

        mSkeleton = DI_NEW DiSkeleton();
        return mSkeleton;
    }

    DiAttachSet* DiMotion::CreateAttachSet()
    {
        SAFE_DELETE(mAttachSet);

        mAttachSet = DI_NEW DiAttachSet();
        return mAttachSet;
    }

    DiAnimation* DiMotion::GetAnimation( const DiString& name )
    {
        AnimationList::const_iterator i = mAnimationList.find(name);

        if (i == mAnimationList.end())
        {
            return NULL;
        }

        return i->second;
    }

    DiAnimation* DiMotion::GetAnimation( const DiString& name ) const
    {
        AnimationList::const_iterator i = mAnimationList.find(name);

        if (i == mAnimationList.end())
        {
            return NULL;
        }

        return i->second;
    }

    DiMotion::AnimationListIterator DiMotion::GetAnimations()
    {
        return DiMotion::AnimationListIterator(mAnimationList.begin(),mAnimationList.end());
    }

    DiMotion::ConstAnimationListIterator DiMotion::GetAnimations() const
    {
        return DiMotion::ConstAnimationListIterator(mAnimationList.begin(),mAnimationList.end());
    }

    DiAnimation* DiMotion::CreateAnimation(const DiString& name, float length)
    {
        if (HasAnimation(name))
        {
            DI_WARNING("The animatin %s has already existed",name.c_str());
            return nullptr;
        }

        DiAnimation* am = DI_NEW DiAnimation(name,length);
        mAnimationList[name] = am;
        return am;
    }

    bool DiMotion::HasAnimation( const DiString& name )
    {
        AnimationList::const_iterator i = mAnimationList.find(name);

        if (i == mAnimationList.end())
        {
            return false;
        }
        return true;
    }

    bool DiMotion::RenameAnimation( const DiString& name,const DiString& strNewName )
    {
        if(strNewName.empty())
        {
            DI_WARNING("Empty name will not be accepted");
            return false;
        }

        AnimationList::iterator it = mAnimationList.find(name);
        if(it != mAnimationList.end())
        {
            DiAnimation * pkAnim = (*it).second;
            mAnimationList.erase(name);

            pkAnim->SetName(strNewName);
            mAnimationList[strNewName] = pkAnim;

            return true;
        }

        DI_WARNING("Cannot locate the animation: %s",name.c_str());
        return false;
    }

    void DiMotion::AssociateNodeAnimToSkeleton(DiSkeleton* skeleton)
    {
        AnimationList::iterator it;
        AnimationList::iterator itEnd = mAnimationList.end();
        for (it = mAnimationList.begin(); it != itEnd; ++it)
        {
            it->second->NodeClipAttachToSkeleton(skeleton);
        }
    }

    void DiMotion::AssociateNodeAnimToAttachSet( DiAttachSet* attachset )
    {
        AnimationList::iterator it;
        AnimationList::iterator itEnd = mAnimationList.end();
        for (it = mAnimationList.begin(); it != itEnd; ++it)
        {
            it->second->NodeClipAttachToAttachSet(attachset);
        }
    }

    void DiMotion::ApplySkeletonAnimation( DiSkeletonInstance* skel, DiClipControllerSet* clipset )
    {
        if (!GetNumAnimations())
        {
            return;
        }

        skel->Reset();

        float weightFactor = 1.0f;
        if (skel->GetBlendMode() == ANIMBLEND_AVERAGE)
        {
            float totalWeights = 0.0f;
            ConstEnabledClipsIt stateIt = 
                clipset->GetEnabledClipsIterator();
            while (stateIt.HasMoreElements())
            {
                const DiClipController* animState = stateIt.GetNext();
                totalWeights += animState->GetWeight();
            }

            if (totalWeights > 1.0f)
            {
                weightFactor = 1.0f / totalWeights;
            }
        }

        ConstEnabledClipsIt stateIt = 
            clipset->GetEnabledClipsIterator();
        while (stateIt.HasMoreElements())
        {
            const DiClipController* animState = stateIt.GetNext();

            DiAnimation* anima = GetAnimation(animState->GetAnimationName());

            if( anima )
            {
                DiTimeIndex timeIndex = anima->GetTimeIndex(animState->GetTimePosition());

                if(animState->HasBoneBlendMask())
                {
                    anima->ApplySkeleton(skel, timeIndex, 
                        animState->GetWeight() * weightFactor,
                        animState->GetBoneBlendMask(), 1.0f);
                }
                else
                {
                    anima->ApplySkeleton(skel, timeIndex, 
                        animState->GetWeight() * weightFactor, 1.0f);
                }
            }
        }
    }
}