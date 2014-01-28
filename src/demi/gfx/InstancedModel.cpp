
#include "GfxPch.h"
#include "InstancedModel.h"
#include "InstanceBatch.h"
#include "ClipController.h"
#include "Animation.h"
#include "AnimationClip.h"
#include "Skeleton.h"
#include "Bone.h"
#include "Model.h"
#include "SubModel.h"
#include "CullNode.h"
#include "OptimisedUtil.h"
#include "GfxDriver.h"
#include "RenderWindow.h"
#include "AlignedAllocator.h"

namespace Demi
{
    DiInstancedModel::DiInstancedModel( DiInstanceBatch *batchOwner, 
        uint32 instanceID, DiInstancedModel* sharedTransformEntity /*= NULL*/ )
        :DiTransformUnit(),
        mInstanceId( instanceID ),
        mInUse( false ),
        mBatchOwner( batchOwner ),
        mClipSet( NULL ),
        mSkeletonInstance( NULL ),
        mBoneMatrices( NULL ),
        mBoneWorldMatrices( NULL ),
        mFrameAnimationLastUpdated(std::numeric_limits<ULONG>::max() - 1),
        mSharedTransformModel( NULL ),
        mTransformLookupNumber(instanceID),
        mPosition(DiVec3::ZERO),
        mDerivedLocalPosition(DiVec3::ZERO),
        mOrientation(DiQuat::IDENTITY),
        mScale(DiVec3::UNIT_SCALE),
        mMaxScaleLocal(1),
        mNeedTransformUpdate(true),
        mNeedAnimTransformUpdate(true),
        mUseLocalTransform(false),
        mSpeed(1),
        mAutoUpdateAnims(false)
    {
        static int count = 0;
        DiString s;
        s.SetInt(count++);
        mName = batchOwner->GetName() + "_InsMdl_";
        mName += s;

        if (sharedTransformEntity)
        {
            sharedTransformEntity->ShareTransformWith(this);
        }
        else
        {
            CreateSkeletonInstance();
        }
        UpdateTransforms();
    }

    DiInstancedModel::~DiInstancedModel()
    {
        UnlinkTransform();
        DestroySkeletonInstance();
    }

    bool DiInstancedModel::ShareTransformWith( DiInstancedModel *slave )
    {
        if( !this->mBatchOwner->HasSkeleton() ||
            !this->mBatchOwner->SupportsSkeletalAnimation() )
        {
            return false;
        }

        if( this->mSharedTransformModel  )
        {
            DI_ERROR("已经共享了其他的实例模型");
            return false;
        }

        if( this->mBatchOwner->GetSkeleton() !=
            slave->mBatchOwner->GetSkeleton() )
        {
            DI_ERROR("骨骼无法匹配");
            return false;
        }

        slave->UnlinkTransform();
        slave->DestroySkeletonInstance();

        slave->mSkeletonInstance    = this->mSkeletonInstance;
        slave->mClipSet    = this->mClipSet;
        slave->mBoneMatrices        = this->mBoneMatrices;
        if (mBatchOwner->UseBoneWorldMatrices())
        {
            slave->mBoneWorldMatrices = this->mBoneWorldMatrices;
        }
        slave->mSharedTransformModel = this;
        this->mSharingPartners.push_back( slave );

        slave->mBatchOwner->MarkTransformSharingDirty();

        return true;
    }

    void DiInstancedModel::StopSharingTransform()
    {
        if( mSharedTransformModel )
        {
            UnlinkTransform();
            CreateSkeletonInstance();
        }
        else
        {
            InstancedEntityVec::const_iterator itor = mSharingPartners.begin();
            InstancedEntityVec::const_iterator end  = mSharingPartners.end();
            while( itor != end )
            {
                (*itor)->StopSharingTransform();
                ++itor;
            }
            mSharingPartners.clear();
        }
    }

    const DiAABB& DiInstancedModel::GetBoundingBox( void ) const
    {
        return mBatchOwner->GetMeshReference()->GetBounds();
    }

    void DiInstancedModel::NotifyAttached( DiNode* parent )
    {
        MarkTransformDirty();
        DiTransformUnit::NotifyAttached( parent );
        UpdateTransforms();
    }

    DiClipController* DiInstancedModel::GetClipController( const DiString& name ) const
    {
        DI_ASSERT(mClipSet);
        return mClipSet->GetClipController(name);
    }

    DiClipControllerSet* DiInstancedModel::GetClipControllerSet( void ) const
    {
        return mClipSet;
    }

    bool DiInstancedModel::UpdateAnimation( void )
    {
        DI_PROFILE(InstancedModel_UpdateAnimation);

        if (mSharedTransformModel)
        {
            return mSharedTransformModel->UpdateAnimation();
        }
        else
        {
            if (mAutoUpdateAnims && mClipSet)
            {
                ConstEnabledClipsIt it = mClipSet->GetEnabledClipsIterator();
                while (it.HasMoreElements())
                {
                    DiClipController* cc = it.GetNext();
                    cc->AddTime(Driver->GetDeltaSecond() * mSpeed);
                }
            }

            const bool animationDirty =
                (mFrameAnimationLastUpdated != mClipSet->GetDirtyFrameNumber());

            if( animationDirty || (mNeedAnimTransformUpdate &&  mBatchOwner->UseBoneWorldMatrices()))
            {
                if ( mSkeletonInstance )
                {
                    mBatchOwner->GetMotionReference()->ApplySkeletonAnimation(
                        mSkeletonInstance,mClipSet);
                    mSkeletonInstance->GetBoneMatrices( mBoneMatrices );

                    if (mBatchOwner->UseBoneWorldMatrices())
                    {
                        DiOptimisedUtil::GetInstance()->ConcatenateAffineMatrices(GetParentNodeFullTransform(),
                            mBoneMatrices,mBoneWorldMatrices,mSkeletonInstance->GetNumBones());

                        mNeedAnimTransformUpdate = false;
                    }
                }
                mFrameAnimationLastUpdated = mClipSet->GetDirtyFrameNumber();

                return true;
            }
        }

        return false;
    }

    void DiInstancedModel::SetInUse( bool used )
    {
        mInUse = used;
        mUseLocalTransform &= used;
    }

    size_t DiInstancedModel::GetTransforms( DiMat4 *xform ) const
    {
        size_t retVal = 1;

        if( GetVisible() )
        {
            if( !mSkeletonInstance )
            {
                *xform = mBatchOwner->UseBoneWorldMatrices() ? 
                    GetParentNodeFullTransform() : DiMat4::IDENTITY;
            }
            else
            {
                DiMat4* matrices = mBatchOwner->UseBoneWorldMatrices() ? mBoneWorldMatrices : mBoneMatrices;

                size_t bones = mSkeletonInstance->GetNumBones();
                for (size_t b=0; b <bones;)
                {
                    *xform++ = matrices[b++];
                }

//                 const DiSubMesh::IndexMap *indexMap = m_pkBatchOwner->GetIndexToBoneMap();
//                 DiSubMesh::IndexMap::const_iterator itor = indexMap->begin();
//                 DiSubMesh::IndexMap::const_iterator end  = indexMap->end();
// 
//                 while( itor != end )
//                 {
//                     *xform++ = matrices[*itor++];
//                 }

                retVal = bones;//indexMap->size();
            }
        }
        else
        {
            if( mSkeletonInstance )
            {
                retVal = mBatchOwner->GetIndexToBoneMap()->size();
            }

            for (size_t i = 0; i < retVal; i++)
            {
                xform[i] = DiMat4::ZEROAFFINE;
            }
        }

        return retVal;
    }

    size_t DiInstancedModel::GetTransforms3x4( float *xform ) const
    {
        size_t retVal;
        if( GetVisible() )
        {
            if( !mSkeletonInstance )
            {
                const DiMat4& mat = mBatchOwner->UseBoneWorldMatrices() ? 
                    GetParentNodeFullTransform() : DiMat4::IDENTITY;
                for( int i=0; i<3; ++i )
                {
                    float const *row = mat[i];
                    for( int j=0; j<4; ++j )
                        *xform++ = *row++;
                }

                retVal = 12;
            }
            else
            {
                DiMat4* matrices = mBatchOwner->UseBoneWorldMatrices() ? mBoneWorldMatrices : mBoneMatrices;

                const DiSubMesh::IndexMap *indexMap = mBatchOwner->GetIndexToBoneMap();
                DiSubMesh::IndexMap::const_iterator itor = indexMap->begin();
                DiSubMesh::IndexMap::const_iterator end  = indexMap->end();

                while( itor != end )
                {
                    const DiMat4 &mat = matrices[*itor++];
                    for( int i=0; i<3; ++i )
                    {
                        float const *row = mat[i];
                        for( int j=0; j<4; ++j )
                        {
                            *xform++ = *row++;
                        }
                    }
                }

                retVal = indexMap->size() * 4 * 3;
            }
        }
        else
        {
            if( mSkeletonInstance )
            {
                retVal = mBatchOwner->GetIndexToBoneMap()->size() * 3 * 4;
            }
            else
            {
                retVal = 12;
            }

            for (size_t i = 0; i < retVal; i++)
            {
                xform[i] = 0;
            }
        }

        return retVal;
    }

    bool DiInstancedModel::FindVisible( DiCamera *camera ) const
    {
        bool retVal = GetVisible();
        float r = DiMath::BoundingRadiusFromAABB(GetBoundingBox());

        if( retVal && camera )
        {
            retVal = camera->IsVisible(DiSphere(GetDerivedPosition(),r));
        }

        return retVal;
    }

    void DiInstancedModel::CreateSkeletonInstance()
    {
        if( mBatchOwner->HasSkeleton() &&
            mBatchOwner->SupportsSkeletalAnimation() )
        {
            mSkeletonInstance = DI_NEW DiSkeletonInstance( mBatchOwner->GetSkeleton() );

            // 注意，要用到SSE指令集优化时，骨骼矩阵和世界骨骼矩阵必须是要16字节对齐的
            mBoneMatrices = static_cast<DiMat4*>(DiAlignedMemory::Allocate( sizeof(DiMat4) *mSkeletonInstance->GetNumBones()));

            if (mBatchOwner->UseBoneWorldMatrices())
            {
                mBoneWorldMatrices    = static_cast<DiMat4*>(DiAlignedMemory::Allocate( sizeof(DiMat4) *
                    mSkeletonInstance->GetNumBones()));
            }

            mClipSet = DI_NEW DiClipControllerSet();

            DiMotion::AnimationListIterator i = mBatchOwner->GetMotionReference()->GetAnimations();
            while (i.HasMoreElements())
            {
                DiAnimation* anim = i.GetNext();
                const DiString& animName = anim->GetName();
                mClipSet->CreateClipController(animName, 0.0, anim->GetLength());
            }
        }
    }

    void DiInstancedModel::DestroySkeletonInstance()
    {
        if( mSkeletonInstance )
        {
            while( mSharingPartners.empty() == false )
            {
                mSharingPartners.front()->StopSharingTransform();
            }
            mSharingPartners.clear();

            DI_DELETE mSkeletonInstance;
            DI_DELETE mClipSet;

            DiAlignedMemory::Deallocate(mBoneMatrices);
            DiAlignedMemory::Deallocate(mBoneWorldMatrices);

            mSkeletonInstance    = NULL;
            mClipSet            = NULL;
            mBoneMatrices        = NULL;
            mBoneWorldMatrices    = NULL;
        }
    }

    void DiInstancedModel::UnlinkTransform()
    {
        if( mSharedTransformModel )
        {
            mSharedTransformModel->NotifyUnlink( this );
            mBatchOwner->MarkTransformSharingDirty();

            mSkeletonInstance        = NULL;
            mClipSet                = NULL;
            mBoneMatrices            = NULL;
            mBoneWorldMatrices        = NULL;
            mSharedTransformModel    = NULL;
        }
    }

    void DiInstancedModel::NotifyUnlink( const DiInstancedModel *slave )
    {
        InstancedEntityVec::iterator itor = mSharingPartners.begin();
        InstancedEntityVec::iterator end  = mSharingPartners.end();
        while( itor != end )
        {
            if( *itor == slave )
            {
                std::swap(*itor,mSharingPartners.back());
                mSharingPartners.pop_back();
                break;
            }

            ++itor;
        }
    }

    void DiInstancedModel::MarkTransformDirty()
    {
        mNeedTransformUpdate = true;
        mNeedAnimTransformUpdate = true; 
        mBatchOwner->BoundsDirty();
    }

    float DiInstancedModel::GetMaxScaleCoef() const
    {
        if (mParentNode)
        {
            const DiVec3& parentScale = mParentNode->GetDerivedScale();
            float p = DiMath::Max(DiMath::Max(DiMath::Abs(parentScale.x), DiMath::Abs(parentScale.y)), DiMath::Abs(parentScale.z));
            return mMaxScaleLocal * p; 
        }
        return mMaxScaleLocal; 
    }

    void DiInstancedModel::SetPosition( const DiVec3& position, bool doUpdate /*= true*/ )
    {
        mPosition = position; 
        mDerivedLocalPosition = position;
        mUseLocalTransform = true;
        MarkTransformDirty();
        if (doUpdate) 
        {
            UpdateTransforms();
        }
    }

    void DiInstancedModel::SetOrientation( const DiQuat& orientation, bool doUpdate /*= true*/ )
    {
        mOrientation = orientation;  
        mUseLocalTransform = true;
        MarkTransformDirty();
        if (doUpdate) 
        {
            UpdateTransforms();
        }
    }

    void DiInstancedModel::SetScale( const DiVec3& scale, bool doUpdate /*= true*/ )
    {
        mScale = scale; 
        mMaxScaleLocal = DiMath::Max(DiMath::Max(
            DiMath::Abs(mScale.x), DiMath::Abs(mScale.y)), DiMath::Abs(mScale.z)); 
        mUseLocalTransform = true;
        MarkTransformDirty();
        if (doUpdate) 
        {
            UpdateTransforms();
        }
    }

    void DiInstancedModel::UpdateTransforms()
    {
        if (mUseLocalTransform && mNeedTransformUpdate)
        {
            if (mParentNode)
            {
                const DiVec3& parentPosition = mParentNode->GetDerivedPosition();
                const DiQuat& parentOrientation = mParentNode->GetDerivedOrientation();
                const DiVec3& parentScale = mParentNode->GetDerivedScale();

                DiQuat derivedOrientation = parentOrientation * mOrientation;
                DiVec3 derivedScale = parentScale * mScale;
                mDerivedLocalPosition = parentOrientation * (parentScale * mPosition) + parentPosition;

                mFullLocalTransform.makeTransform(mDerivedLocalPosition, derivedScale, derivedOrientation);
            }
            else
            {
                mFullLocalTransform.makeTransform(mPosition,mScale,mOrientation);
            }
            mNeedTransformUpdate = false;
        }
    }

    const DiMat4& DiInstancedModel::GetParentNodeFullTransform( void ) const
    {
        DI_ASSERT((!mNeedTransformUpdate || !mUseLocalTransform));
        return mUseLocalTransform ? mFullLocalTransform :
            mParentNode ? mParentNode->GetFullTransform() : DiMat4::IDENTITY;
    }

    const DiVec3& DiInstancedModel::GetDerivedPosition() const
    {
        DI_ASSERT((!mNeedTransformUpdate || !mUseLocalTransform));
        return mUseLocalTransform ? mDerivedLocalPosition :
            mParentNode ? mParentNode->GetDerivedPosition() : DiVec3::ZERO;
    }

    DiString& DiInstancedModel::GetType()
    {
        static DiString type = "InsModel";
        return type;
    }
}