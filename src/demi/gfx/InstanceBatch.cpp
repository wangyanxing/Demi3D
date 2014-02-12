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
#include "InstanceBatch.h"
#include "CullNode.h"
#include "RenderBatchGroup.h"
#include "InstancedModel.h"
#include "InstanceManager.h"

namespace Demi
{
    DiInstanceBatch::DiInstanceBatch( DiInstanceManager*creator, DiMeshPtr meshReference,
        DiMotionPtr motionReference, const DiMaterialPtr &material, size_t instancesPerBatch, 
        const DiSubMesh::IndexMap *indexToBoneMap, const DiString &batchName )
        :mInstancesPerBatch( instancesPerBatch ),
        mCreator( creator ),
        mMeshReference( meshReference ),
        mMotionReference( motionReference ),
        mIndexToBoneMap( indexToBoneMap ),
        mBoundsDirty( false ),
        mBoundsUpdated( false ),
        mCurrentCamera( 0 ),
        mTechnSupportsSkeletal( true ),
        mCachedCamera( 0 ),
        mTransformSharingDirty(true),
        mRemoveOwnVertexData(false),
        mRemoveOwnIndexData(false),
        mAddToBatch(false)
    {
        mMaterial = material;
        DI_ASSERT(mInstancesPerBatch);

        mFullBoundingBox.SetExtents( -DiVec3::ZERO, DiVec3::ZERO );
        mPrimitiveType = PT_TRIANGLELIST;

        mName = batchName;
    }

    DiInstanceBatch::~DiInstanceBatch()
    {
        DeleteAllInstancedModels();

        DiCullNode *sceneNode = GetParentCullNode();
        if( sceneNode )
        {
            sceneNode->DetachAllObjects();
            sceneNode->GetParent()->RemoveChild( sceneNode->GetName() );
        }

        if( mRemoveOwnVertexData )
        {
            ReleaseSourceData();
        }
        if( mRemoveOwnIndexData )
        {
            ReleaseIndexBuffer();
        }
        ReleaseVertexDeclaration();
    }

    void DiInstanceBatch::SetInstancesPerBatch( size_t instancesPerBatch )
    {
        if( !mInstancedModels.empty() )
        {
            DI_ERROR("SetInstancesPerBatch Error: empty instanced model");
        }

        mInstancesPerBatch = instancesPerBatch;
    }

    void DiInstanceBatch::UpdateBounds( void )
    {
        mFullBoundingBox.SetNull();

        InstancedModelVec::const_iterator itor = mInstancedModels.begin();
        InstancedModelVec::const_iterator end  = mInstancedModels.end();

        float maxScale = 0;
        while( itor != end )
        {
            DiInstancedModelPtr md = (*itor);
            maxScale = DiMath::Max(maxScale, md->GetMaxScaleCoef());
            mFullBoundingBox.Merge( md->GetDerivedPosition() );

            ++itor;
        }

        float addToBound = maxScale * DiMath::BoundingRadiusFromAABB(GetMeshReference()->GetBounds());
        mFullBoundingBox.SetMaximum(mFullBoundingBox.GetMaximum() + addToBound);
        mFullBoundingBox.SetMinimum(mFullBoundingBox.GetMinimum() - addToBound);

        if (GetParentCullNode())
        {
            GetParentCullNode()->NeedUpdate(true);
        }

        mBoundsDirty    = false;
        mBoundsUpdated    = true;
    }

    void DiInstanceBatch::Build( const DiSubMesh* baseSubMesh )
    {
        if( CheckSubMeshCompatibility( baseSubMesh ) )
        {
            SetupVertices( baseSubMesh );
            SetupIndices( baseSubMesh );

            CreateAllInstancedModels();
        }
    }

    void DiInstanceBatch::BuildFrom( const DiSubMesh *baseSubMesh )
    {
        Build(baseSubMesh);
    }

    void DiInstanceBatch::GetInstancedEntitiesInUse( InstancedModelVec &outEntities )
    {
        InstancedModelVec::const_iterator itor = mInstancedModels.begin();
        InstancedModelVec::const_iterator end  = mInstancedModels.end();

        while( itor != end )
        {
            if( (*itor)->IsInUse() )
                outEntities.push_back( *itor );
            ++itor;
        }
    }

    void DiInstanceBatch::DefragmentBatch( bool optimizeCulling, InstancedModelVec &usedEntities )
    {
        mInstancedModels.clear();
        DeleteUnusedInstancedModels();

        if( !optimizeCulling )
        {
            DefragmentBatchNoCull( usedEntities );
        }
        else
        {
            DefragmentBatchDoCull( usedEntities );
        }

        uint32 instanceId = 0;
        InstancedModelVec::const_iterator itor = mInstancedModels.begin();
        InstancedModelVec::const_iterator end  = mInstancedModels.end();

        while( itor != end )
        {
            (*itor)->mInstanceId = instanceId++;
            (*itor)->mBatchOwner = this;
            ++itor;
        }

        DI_ASSERT( (signed)(mInstancesPerBatch) - (signed)(mInstancedModels.size()) >= 0 );
        mInstancedModels.reserve( mInstancesPerBatch );
        mUnusedModels.reserve( mInstancesPerBatch );
        for( size_t i = mInstancedModels.size(); i<mInstancesPerBatch; ++i )
        {
            DiInstancedModelPtr instance = GenerateInstancedModel(i);
            mInstancedModels.push_back( instance );
            mUnusedModels.push_back( instance );
        }

        if( !IsBatchUnused() )
        {
            BoundsDirty();
        }
    }

    void DiInstanceBatch::BoundsDirty( void )
    {
        if( mCreator && !mBoundsDirty ) 
        {
            mCreator->AddDirtyBatch(std::dynamic_pointer_cast<DiInstanceBatch>(shared_from_this()));
        }
        mBoundsDirty = true;
    }

    DiInstancedModelPtr DiInstanceBatch::CreateInstancedModel()
    {
        DiInstancedModelPtr retVal = 0;

        if( !mUnusedModels.empty() )
        {
            retVal = mUnusedModels.back();
            mUnusedModels.pop_back();

            retVal->SetInUse(true);
        }

        return retVal;
    }

    void DiInstanceBatch::RemoveInstancedModel( DiInstancedModelPtr instancedModel )
    {
        DI_ASSERT( instancedModel->mBatchOwner == this );

        if( instancedModel->GetParentCullNode() )
        {
            instancedModel->GetParentCullNode()->DetachObject( instancedModel );
        }

        instancedModel->SetInUse(false);
        instancedModel->StopSharingTransform();

        mUnusedModels.push_back( instancedModel );
    }

    void DiInstanceBatch::NotifyCurrentCamera( DiCamera* cam )
    {
        mCurrentCamera = cam;
    }

    const DiAABB& DiInstanceBatch::GetBoundingBox( void ) const
    {
        return mFullBoundingBox;
    }

    void DiInstanceBatch::AddToBatchGroup(DiRenderBatchGroup* bg)
    {
        if (mAddToBatch)
        {
            bg->AddRenderUnit(this);
            mAddToBatch = false;
        }
    }

    void DiInstanceBatch::Update(DiCamera* camera)
    {
        mCurrentCamera = camera;
        mDirtyAnimation = false;

        UpdateVisibility();

        mPrimitiveCount = mMeshReference->GetSubMesh(mCreator->mSubMeshIdx)->GetPrimitiveCount();
        mPrimitiveCount *= GetModelNums();

        if (mVisible)
        {
            if (HasSkeleton())
            {
                auto itor = mInstancedModels.begin();
                auto end = mInstancedModels.end();

                while (itor != end)
                {
                    mDirtyAnimation |= (*itor)->UpdateAnimation();
                    ++itor;
                }
            }

            mAddToBatch = true;
        }

        mVisible = true;
    }

    void DiInstanceBatch::CullingUpdate( DiRenderBatchGroup* group, DiCamera* )
    {
        mDirtyAnimation = false;

        UpdateVisibility();

        mPrimitiveCount = mMeshReference->GetSubMesh(mCreator->mSubMeshIdx)->GetPrimitiveCount();
        mPrimitiveCount *= GetModelNums();

        if( mVisible )
        {
            if (HasSkeleton())
            {
                InstancedModelVec::const_iterator itor = mInstancedModels.begin();
                InstancedModelVec::const_iterator end  = mInstancedModels.end();

                while( itor != end )    
                {
                    mDirtyAnimation |= (*itor)->UpdateAnimation();
                    ++itor;
                }
            }

            group->AddRenderUnit( this );
        }

        mVisible = true;
    }

    void DiInstanceBatch::CreateAllInstancedModels( void )
    {
        mInstancedModels.reserve( mInstancesPerBatch );
        mUnusedModels.reserve( mInstancesPerBatch );

        for( size_t i=0; i<mInstancesPerBatch; ++i )
        {
            DiInstancedModelPtr instance = GenerateInstancedModel(i);
            mInstancedModels.push_back( instance );
            mUnusedModels.push_back( instance );
        }
    }

    void DiInstanceBatch::DeleteAllInstancedModels( void )
    {
        auto itor = mInstancedModels.begin();
        auto end  = mInstancedModels.end();

        while( itor != end )
        {
            if( (*itor)->GetParentCullNode() )
            {
                (*itor)->GetParentCullNode()->DetachObject( (*itor) );
            }

            itor->reset();
            itor++;
        }
    }

    void DiInstanceBatch::DeleteUnusedInstancedModels( void )
    {
        auto itor = mUnusedModels.begin();
        auto end  = mUnusedModels.end();

        while( itor != end )
        {
            itor->reset();
            itor++;
        }

        mUnusedModels.clear();
    }

    DiInstancedModelPtr DiInstanceBatch::GenerateInstancedModel( size_t num )
    {
        return make_shared<DiInstancedModel>( this, num);
    }

    void DiInstanceBatch::MakeMatrixCameraRelative3x4( float *mat3x4, size_t numFloats )
    {
        const DiVec3 &cameraRelativePosition = mCurrentCamera->GetDerivedPosition();

        for( size_t i=0; i<numFloats >> 2; i += 3 )
        {
            const DiVec3 worldTrans( mat3x4[(i+0) * 4 + 3], mat3x4[(i+1) * 4 + 3],
                mat3x4[(i+2) * 4 + 3] );
            const DiVec3 newPos( worldTrans - cameraRelativePosition );

            mat3x4[(i+0) * 4 + 3] = newPos.x;
            mat3x4[(i+1) * 4 + 3] = newPos.y;
            mat3x4[(i+2) * 4 + 3] = newPos.z;
        }
    }

    bool DiInstanceBatch::CheckSubMeshCompatibility( const DiSubMesh* baseSubMesh )
    {
        if (mPrimitiveType != PT_TRIANGLELIST)
        {
            DI_WARNING("Only trangle list can be supported.");
            return false;
        }
        return true;
    }

    void DiInstanceBatch::UpdateVisibility( void )
    {
        mVisible = false;

        InstancedModelVec::const_iterator itor = mInstancedModels.begin();
        InstancedModelVec::const_iterator end  = mInstancedModels.end();

        while( itor != end )
        {
            mVisible |= (*itor)->FindVisible( mCurrentCamera );
            ++itor;
        }
    }

    void DiInstanceBatch::DefragmentBatchNoCull( InstancedModelVec &usedEntities )
    {
        const uint32 maxInstancesToCopy = DiMath::Min( mInstancesPerBatch, usedEntities.size() );
        InstancedModelVec::iterator first = usedEntities.end() - maxInstancesToCopy;

        mInstancedModels.insert( mInstancedModels.begin(), first, usedEntities.end() );
        usedEntities.resize( usedEntities.size() - maxInstancesToCopy );    
    }

    void DiInstanceBatch::DefragmentBatchDoCull( InstancedModelVec &usedEntities )
    {
        InstancedModelVec::const_iterator itor   = usedEntities.begin();
        InstancedModelVec::const_iterator end   = usedEntities.end();

        DiVec3 vMinPos = DiVec3::ZERO, firstPos = DiVec3::ZERO;
        DiInstancedModelPtr first;

        if( !usedEntities.empty() )
        {
            first    = *usedEntities.begin();
            firstPos= first->GetDerivedPosition();
            vMinPos = first->GetDerivedPosition();
        }

        while( itor != end )
        {
            const DiVec3 &vPos = (*itor)->GetDerivedPosition();

            vMinPos.x = DiMath::Min( vMinPos.x, vPos.x );
            vMinPos.y = DiMath::Min( vMinPos.y, vPos.y );
            vMinPos.z = DiMath::Min( vMinPos.z, vPos.z );
            if( vMinPos.squaredDistance( vPos ) < vMinPos.squaredDistance( firstPos ) )
            {
                firstPos   = vPos;
            }
            ++itor;
        }

        while( !usedEntities.empty() && mInstancedModels.size() < mInstancesPerBatch )
        {
            InstancedModelVec::iterator closest= usedEntities.begin();
            InstancedModelVec::iterator it        = usedEntities.begin();
            InstancedModelVec::iterator e        = usedEntities.end();

            DiVec3 closestPos;
            closestPos = (*closest)->GetDerivedPosition();

            while( it != e )
            {
                const DiVec3 &vPos = (*it)->GetDerivedPosition();
                if( firstPos.squaredDistance( vPos ) < firstPos.squaredDistance( closestPos ) )
                {
                    closest      = it;
                    closestPos   = vPos;
                }
                ++it;
            }

            mInstancedModels.push_back( *closest );

            *closest = *(usedEntities.end() - 1);
            usedEntities.pop_back();
        }
    }

    bool DiInstanceBatch::HasSkeleton() const
    {
        if(!mMotionReference)
        {
            return false;
        }
        else
        {
            return mMotionReference->HasSkeleton();
        }
    }

    DiSkeleton* DiInstanceBatch::GetSkeleton()
    {
        if (!HasSkeleton())
        {
            return NULL;
        }
        return mMotionReference->GetSkeleton();
    }

    void DiInstanceBatch::DefragmentBatchDiscard()
    {
        mInstancedModels.clear();
        DeleteUnusedInstancedModels();
    }

    uint32 DiInstanceBatch::GetModelNums()
    {
        return mInstancedModels.size() - mUnusedModels.size();
    }

    DiString& DiInstanceBatch::GetType()
    {
        static DiString type = "InsBatch";
        return type;
    }
}