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
#include "Skeleton.h"
#include "Bone.h"

namespace Demi
{
    DiSkeleton::DiSkeleton()
        :mNextAutoHandle(0),
        //mIsBlending(false),
        mBlendMode(ANIMBLEND_AVERAGE)
    {

    }

    DiSkeleton::~DiSkeleton()
    {
        BoneList::iterator i;
        for (i = mBoneList.begin(); i != mBoneList.end(); ++i)
        {
            DI_DELETE *i;
        }
        mBoneList.clear();
        mBoneListByName.clear();
        mRootBones.clear();
    }

    void DiSkeleton::Reset()
    {
        BoneList::iterator i;
        for (i = mBoneList.begin(); i != mBoneList.end(); ++i)
        {
            (*i)->Reset();
        }
    }

    DiBone* DiSkeleton::GetRootBone()
    {
        if (mRootBones.empty())
        {
            DeriveRootBone();
        }

        if (!mRootBones.empty())
        {
            return mRootBones[0];
        }
        else
        {
            return nullptr;
        }
    }

    DiBone* DiSkeleton::GetBone( unsigned handle )
    {
        DI_ASSERT(handle < mBoneList.size());
        return mBoneList[handle];
    }

    DiBone* DiSkeleton::GetBone( const DiString& boneName )
    {
        auto i = mBoneListByName.find(boneName);
        if (i == mBoneListByName.end())
        {
            DI_WARNING("Cannot locate the bone: %s",boneName.c_str());
            return nullptr;
        }

        return i->second;
    }

    const DiBone* DiSkeleton::GetBone( unsigned handle ) const
    {
        DI_ASSERT(handle < mBoneList.size());
        return mBoneList[handle];
    }

    bool DiSkeleton::HasBone( const DiString& boneName ) const
    {
        return  mBoneListByName.end() != mBoneListByName.find(boneName);
    }

    DiBone* DiSkeleton::CreateBone()
    {
        return CreateBone(mNextAutoHandle++);
    }

    DiBone* DiSkeleton::CreateBone( uint32 handle )
    {
        if (handle > MAX_BONE_NUM)
        {
            DI_WARNING("Too many bones: %d", handle);
            return nullptr;
        }

        if (handle < mBoneList.size() && mBoneList[handle])
        {
            DI_ERROR("The bone: %d has already existed",handle);
            return nullptr;
        }

        DiBone* ret = DI_NEW DiBone(this,handle);
        assert(mBoneListByName.find(ret->GetName()) == mBoneListByName.end());
        if (mBoneList.size() <= handle)
        {
            mBoneList.resize(handle+1);
        }
        mBoneList[handle] = ret;
        mBoneListByName[ret->GetName()] = ret;
        return ret;
    }

    DiBone* DiSkeleton::CreateBone( const DiString& name )
    {
        return CreateBone(name, mNextAutoHandle++);
    }

    DiBone* DiSkeleton::CreateBone( const DiString& name, uint32 handle )
    {
        if (handle > MAX_BONE_NUM)
        {
            DI_WARNING("Too many bones: %d", handle);
            return nullptr;
        }
        
        if (handle < mBoneList.size() && mBoneList[handle])
        {
            DI_ERROR("The bone: %d has already existed",handle);
            return nullptr;
        }

        if (mBoneListByName.find(name) != mBoneListByName.end())
        {
            DI_WARNING("Cannot locate the bone: %s",name.c_str());
            return nullptr;
        }

        DiBone* ret = DI_NEW DiBone(this, handle, name);
        if (mBoneList.size() <= handle)
        {
            mBoneList.resize(handle+1);
        }
        mBoneList[handle] = ret;
        mBoneListByName[name] = ret;
        return ret;
    }

    void DiSkeleton::DeriveRootBone() const
    {
        if (mBoneList.empty())
        {
            DI_WARNING("The bone list is empty!");
            return;
        }

        mRootBones.clear();

        DiBone* currentBone;
        BoneList::const_iterator i;
        BoneList::const_iterator iend = mBoneList.end();
        for (i = mBoneList.begin(); i != iend; ++i)
        {
            currentBone = *i;
            if (currentBone->GetParent() == 0)
            {
                mRootBones.push_back(currentBone);
            }
        }
    }

    void DiSkeleton::UpdateTransforms( void )
    {
        BoneList::iterator i, iend;
        iend = mRootBones.end();
        for (i = mRootBones.begin(); i != iend; ++i)
        {
            (*i)->_Update(true, false);
        }
    }

    void DiSkeleton::SetBindingPose( void )
    {
        UpdateTransforms();

        BoneList::iterator i;
        for (i = mBoneList.begin(); i != mBoneList.end(); ++i)
        {            
            (*i)->SetBindingPose();
        }
    }

    void DiSkeleton::GetBoneMatrices( DiMat4* pMatrices, bool updateTransform )
    {
        if (updateTransform)
        {
            UpdateTransforms();
        }

        for (auto i = mBoneList.begin();i != mBoneList.end(); ++i)
        {
            DiBone* pBone = *i;
            pBone->GetOffsetTransform(*pMatrices);
            pMatrices++;
        }
    }

    DiSkeleton::BoneIterator DiSkeleton::GetRootBoneIterator( void )
    {
        if (mRootBones.empty())
        {
            DeriveRootBone();
        }
        return BoneIterator(mRootBones.begin(), mRootBones.end());
    }

    //////////////////////////////////////////////////////////////////////////

    DiSkeletonInstance::DiSkeletonInstance( DiSkeleton* parent )
        :mParent(parent)
    {
        Init();
    }

    DiSkeletonInstance::~DiSkeletonInstance()
    {

    }

    void DiSkeletonInstance::Init()
    {
        mNextAutoHandle = mParent->mNextAutoHandle;

        BoneIterator i = mParent->GetRootBoneIterator();
        while (i.HasMoreElements())
        {
            DiBone* b = i.GetNext();
            CloneBone(b,NULL);
            b->_Update(true,false);
        }

        SetBindingPose();
    }

    void DiSkeletonInstance::CloneBone( DiBone* source, DiBone* parent )
    {
        DiBone* newBone;
        if (source->GetName().empty())
            newBone = CreateBone(source->GetHandle());
        else
            newBone = CreateBone(source->GetName(), source->GetHandle());

        if (!parent)
            mRootBones.push_back(newBone);
        else
            parent->AddChild(newBone);
        
        newBone->SetOrientation(source->GetOrientation());
        newBone->SetPosition(source->GetPosition());
        newBone->SetScale(source->GetScale());

        size_t childrenSize = source->GetChildrenNum();
        for (size_t i = 0; i < childrenSize; ++i)
        {
            DiNode * nd = source->GetChild(i);
            if(DiNode::NT_BONENODE != nd->GetNodeType())
                continue;

            CloneBone(static_cast<DiBone*>(nd), newBone);
        }
    }
}