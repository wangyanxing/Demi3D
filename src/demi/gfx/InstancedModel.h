
/********************************************************************
    File:       InstancedModel.h
    Creator:    demiwangya
*********************************************************************/

#pragma once

#include "TransformUnit.h"

namespace Demi
{
    typedef shared_ptr<DiInstancedModel> DiInstancedModelPtr;

    class DI_GFX_API DiInstancedModel : public DiTransformUnit
    {
    public:

        friend class             DiInstanceBatch;
        
        friend class             DiInstanceBatchShader;

        friend class             DiInstanceBatchHardware;

    public:

        DiInstancedModel( DiInstanceBatch *batchOwner, uint32 instanceID, DiInstancedModel* sharedTransformEntity = NULL);

        virtual                 ~DiInstancedModel();

    public:

        bool                    ShareTransformWith( DiInstancedModel *slave );

        void                    StopSharingTransform();

        DiInstanceBatch*        GetOwner() const { return mBatchOwner; }

        const DiAABB&           GetBoundingBox(void) const;

        void                    NotifyAttached( DiNode* parent );

        bool                    HasSkeleton(void) const { return mSkeletonInstance != 0; }

        DiSkeletonInstance*     GetSkeleton(void) const { return mSkeletonInstance; }

        DiClipController*       GetClipController(const DiString& name) const;

        DiClipControllerSet*    GetClipControllerSet(void) const;

        virtual bool            UpdateAnimation(void);

        void                    SetTransformLookupNumber(uint16 num) { mTransformLookupNumber = num;}

        const DiVec3&           GetPosition() const { return mPosition; }

        void                    SetPosition(const DiVec3& position, bool doUpdate = true);

        const DiQuat&           GetOrientation() const { return mOrientation; }

        void                    SetOrientation(const DiQuat& orientation, bool doUpdate = true);

        const DiVec3&           GetScale() const { return mScale; }

        void                    SetScale(const DiVec3& scale, bool doUpdate = true);

        float                   GetMaxScaleCoef() const;

        void                    UpdateTransforms();

        bool                    IsInUse() const { return mInUse; }

        void                    SetInUse(bool used);

        virtual const DiMat4&   GetParentNodeFullTransform(void) const;

        const DiVec3&           GetDerivedPosition() const;

        bool                    IsAutoUpdateAnims() const { return mAutoUpdateAnims; }

        void                    SetAutoUpdateAnims(bool val) { mAutoUpdateAnims = val; }

        float                   GetSpeed() const { return mSpeed; }

        void                    SetSpeed(float val) { mSpeed = val; }

        DiString&               GetType();

    protected:

        size_t                  GetTransforms( DiMat4 *xform ) const;

        size_t                  GetTransforms3x4( float *xform ) const;

        bool                    FindVisible( DiCamera *camera ) const;

        void                    CreateSkeletonInstance();

        void                    DestroySkeletonInstance();

        void                    UnlinkTransform();

        void                    NotifyUnlink( const DiInstancedModel *slave );

        inline void             MarkTransformDirty();

    protected:

        uint32                  mInstanceId;

        bool                    mInUse;
        
        DiInstanceBatch*        mBatchOwner;

        DiClipControllerSet*    mClipSet;
        
        DiSkeletonInstance*     mSkeletonInstance;
        
        DiMat4*                 mBoneMatrices;
        
        DiMat4*                 mBoneWorldMatrices;

        uint64                  mFrameAnimationLastUpdated;

        DiInstancedModel*       mSharedTransformModel;

        uint32                  mTransformLookupNumber;

        typedef DiVector<DiInstancedModel*> InstancedEntityVec;
        InstancedEntityVec      mSharingPartners;

        DiVec3                  mPosition;

        DiVec3                  mDerivedLocalPosition;

        DiQuat                  mOrientation;

        DiVec3                  mScale;

        float                   mMaxScaleLocal;

        DiMat4                  mFullLocalTransform;

        bool                    mNeedTransformUpdate;

        bool                    mNeedAnimTransformUpdate;

        bool                    mUseLocalTransform;

        float                   mSpeed;
    
        bool                    mAutoUpdateAnims;
        
    };
}