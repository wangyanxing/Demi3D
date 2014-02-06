
/********************************************************************
    File:       Skeleton.h
    Creator:    demiwangya
*********************************************************************/

#pragma once

namespace Demi
{
    enum SkeletonBlendMode 
    {
        ANIMBLEND_AVERAGE = 0,
        ANIMBLEND_CUMULATIVE = 1
    };

    class DI_GFX_API DiSkeleton : public DiBase
    {
    public:

        DiSkeleton();

        virtual                ~DiSkeleton();

    public:

        typedef DiVector<DiBone*> BoneList;

        typedef DiVectorIterator<BoneList> BoneIterator;

        typedef DiMap<DiString,DiBone*> BoneNameMap;

        friend class DiSkeletonInstance;

    public:

        void                Reset();

        const BoneList&     GetBones() const { return mBoneList; }
        
        BoneList&           GetBones() { return mBoneList; }
        
        size_t              GetNumBones() const { return mBoneList.size(); }

        DiBone*             CreateBone();

        DiBone*             CreateBone(uint32 handle);

        DiBone*             CreateBone(const DiString& name);

        DiBone*             CreateBone(const DiString& name, uint32 handle);
        
        DiBone*             GetRootBone();
        
        DiBone*             GetBone(unsigned handle);

        const DiBone*       GetBone(unsigned handle)const;
        
        DiBone*             GetBone(const DiString& boneName);

        bool                HasBone(const DiString& name) const;

        BoneIterator        GetRootBoneIterator(void);

        void                UpdateTransforms(void);

        void                SetBindingPose(void);

        void                GetBoneMatrices(Demi::DiMat4* pMatrices, bool updateTransform = true);

        SkeletonBlendMode   GetBlendMode() const { return mBlendMode; }

        void                SetBlendMode(SkeletonBlendMode val) { mBlendMode = val; }

    protected:

        void                DeriveRootBone() const;

    private:
        
        mutable BoneList    mBoneList;

        mutable BoneList    mRootBones;

        BoneNameMap         mBoneListByName;
        
        uint32              mNextAutoHandle;

        SkeletonBlendMode   mBlendMode;
        
        //bool                mIsBlending;
    };

    class DiSkeletonInstance : public DiSkeleton
    {
    public:

        DiSkeletonInstance(DiSkeleton* parent);

        ~DiSkeletonInstance();

    protected:

        void                Init();

        void                CloneBone(DiBone* source, DiBone* parent);

    protected:

        DiSkeleton*         mParent;
    };
}