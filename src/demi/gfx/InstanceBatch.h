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

#ifndef DiInstanceBatch_h__
#define DiInstanceBatch_h__

#include "RenderUnit.h"
#include "TransformUnit.h"
#include "Mesh.h"
#include "SubMesh.h"
#include "Motion.h"
#include "InstancedModel.h"

namespace Demi
{
    class DiInstanceBatch : public DiRenderUnit, public DiTransformUnit
    {
    public:

        typedef DiVector<DiInstancedModelPtr> InstancedModelVec;

        DiInstanceBatch( DiInstanceManager*creator, DiMeshPtr meshReference,
                                    DiMotionPtr motionReference,
                                    const DiMaterialPtr &material,
                                    size_t instancesPerBatch, 
                                    const DiSubMesh::IndexMap *indexToBoneMap,
                                    const DiString &batchName );

        virtual                        ~DiInstanceBatch();

    public:

        void                        SetInstancesPerBatch( size_t instancesPerBatch );

        const DiSubMesh::IndexMap*  GetIndexToBoneMap() const { return mIndexToBoneMap; }

        bool                        SupportsSkeletalAnimation() const { return mTechnSupportsSkeletal; }

        void                        UpdateBounds(void);

        virtual uint32              CalculateMaxNumInstances( const DiSubMesh *baseSubMesh ) const = 0;

        virtual void                Build( const DiSubMesh* baseSubMesh );

        virtual void                BuildFrom( const DiSubMesh *baseSubMesh);

        const DiMeshPtr&            GetMeshReference() const { return mMeshReference; }

        const DiMotionPtr&          GetMotionReference() const { return mMotionReference; }

        bool                        HasSkeleton() const;

        DiSkeleton*                 GetSkeleton();

        bool                        IsBatchFull(void) const { return mUnusedModels.empty(); }

        bool                        IsBatchUnused(void) const { return mUnusedModels.size() == mInstancedModels.size(); }

        void                        GetInstancedEntitiesInUse( InstancedModelVec &outEntities );

        void                        DefragmentBatch( bool optimizeCulling, InstancedModelVec &usedEntities );

        void                        DefragmentBatchDiscard();

        virtual void                BoundsDirty(void);

        virtual void                SetStaticAndUpdate( bool bStatic ){}

        virtual bool                IsStatic() const { return false; }

        DiInstancedModelPtr         CreateInstancedModel();

        void                        RemoveInstancedModel( DiInstancedModelPtr instancedEntity );

        virtual bool                UseBoneWorldMatrices() const { return true; }

        void                        MarkTransformSharingDirty() { mTransformSharingDirty = true; }

        void                        NotifyCurrentCamera( DiCamera* cam );

        const DiAABB&               GetBoundingBox(void) const;

        virtual void                AddToBatchGroup(DiRenderBatchGroup* bg);

        virtual void                Update(DiCamera* camera);

        uint32                      GetModelNums();

        DiString&                   GetType();

    protected:

        virtual void                SetupVertices( const DiSubMesh* baseSubMesh ) = 0;

        virtual void                SetupIndices( const DiSubMesh* baseSubMesh ) = 0;

        virtual void                CreateAllInstancedModels(void);

        virtual void                DeleteAllInstancedModels(void);

        virtual void                DeleteUnusedInstancedModels(void);

        virtual DiInstancedModelPtr GenerateInstancedModel(size_t num);

        void                        MakeMatrixCameraRelative3x4( float *mat3x4, size_t numFloats );

        virtual bool                CheckSubMeshCompatibility( const DiSubMesh* baseSubMesh );

        void                        UpdateVisibility(void);

        void                        DefragmentBatchNoCull( InstancedModelVec &usedEntities );

        void                        DefragmentBatchDoCull( InstancedModelVec &usedEntities );

    protected:

        size_t                      mInstancesPerBatch;

        DiInstanceManager*          mCreator;

        DiMeshPtr                   mMeshReference;

        DiMotionPtr                 mMotionReference;

        DiSubMesh::IndexMap const*  mIndexToBoneMap;

        InstancedModelVec           mInstancedModels;

        InstancedModelVec           mUnusedModels;

        DiAABB                      mFullBoundingBox;

        bool                        mBoundsDirty;

        bool                        mBoundsUpdated;

        DiCamera*                   mCurrentCamera;

        bool                        mDirtyAnimation;

        bool                        mTechnSupportsSkeletal;

        mutable float               mCachedCameraDist;

        mutable const DiCamera*     mCachedCamera;

        bool                        mTransformSharingDirty;

        bool                        mRemoveOwnVertexData;
        
        bool                        mRemoveOwnIndexData;

        bool                        mAddToBatch;
    };
}

#endif
