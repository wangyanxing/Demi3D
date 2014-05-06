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
#include "InstanceManager.h"
#include "InstanceBatch.h"
#include "InstancedModel.h"
#include "InstanceBatchShader.h"
#include "InstanceBatchHardware.h"
#include "AssetManager.h"
#include "SceneManager.h"
#include "CullNode.h"

namespace Demi
{
    DiInstanceManager::DiInstanceManager( DiSceneManager* sm,const DiString& idname, const DiString& meshName, 
        DiInstanceType insTech, uint32 inssPerBatch )
        :mName( idname ),
        mIdCount( 0 ),
        mInstancesPerBatch( inssPerBatch ),
        mInstancingTechnique( insTech ),
        mSubMeshIdx( 0 ),
        mSceneManager( sm ),
        mMaxLookupTableInstances(16)
    {
        mMeshReference = DiAssetManager::GetInstance().GetAsset<DiMesh>( meshName );

        // ����Ȩ�ش���
    }

    DiInstanceManager::DiInstanceManager( DiSceneManager* sm, const DiString& idname, 
        const DiString& meshName,const DiString& motionName, DiInstanceType insTech,uint32 inssPerBatch )
        :mName( idname ),
        mIdCount( 0 ),
        mInstancesPerBatch( inssPerBatch ),
        mInstancingTechnique( insTech ),
        mSubMeshIdx( 0 ),
        mSceneManager( sm ),
        mMaxLookupTableInstances(16)
    {
        mMeshReference = DiAssetManager::GetInstance().GetAsset<DiMesh>( meshName );
        mMotionReference = DiAssetManager::GetInstance().GetAsset<DiMotion>( motionName );
    }

    DiInstanceManager::~DiInstanceManager()
    {
        auto itor = mInstanceBatches.begin();
        auto end  = mInstanceBatches.end();

        while( itor != end )
        {
            auto it = itor->second.begin();
            auto en = itor->second.end();

            while( it != en )
            {
                it->reset();
                it++;
            }

            ++itor;
        }
    }

    void DiInstanceManager::SetInstancesPerBatch( size_t instancesPerBatch )
    {
        if( !mInstanceBatches.empty() )
        {
            DI_ERROR("Error: SetInstancesPerBatch(), instance batch empty");
            return;
        }

        mInstancesPerBatch = instancesPerBatch;
    }

    void DiInstanceManager::SetMaxLookupTableInstances( size_t maxLookupTableInstances )
    {
        if( !mInstanceBatches.empty() )
        {
            DI_ERROR("Error: SetInstancesPerBatch(), instance batch empty");
            return;
        }

        mMaxLookupTableInstances = maxLookupTableInstances;
    }

    size_t DiInstanceManager::GetMaxOrBestNumInstancesPerBatch( const DiString& materialName, size_t suggestedSize, uint16 flags )
    {
        DiMaterialPtr mat = DiAssetManager::GetInstance().GetAsset<DiMaterial>( materialName );
        DiInstanceBatch *batch = nullptr;

        if( !mat )
            return 0;

        switch( mInstancingTechnique )
        {
        case INSTANCE_SHADER_BASED:
            batch = DI_NEW DiInstanceBatchShader( this, mMeshReference, mMotionReference, mat, suggestedSize,
                0, mName + "_TempBatch" );
            break;
        case INSTANCE_HARDWARE_BASED:
             batch = DI_NEW DiInstanceBatchHardware( this, mMeshReference, mat, suggestedSize,
                  mName + "_TempBatch" );
            break;
        default:
            DI_WARNING("Unsupported instancing technique: %d", mInstancingTechnique);
            break;
        }

        const size_t retVal = batch->CalculateMaxNumInstances( mMeshReference->GetSubMesh(mSubMeshIdx) );

        DI_DELETE batch;

        return retVal;
    }

    DiInstancedModelPtr DiInstanceManager::CreateInstancedModel( const DiString& materialName )
    {
        DiInstanceBatchPtr instanceBatch;

        DiString matname = SetupMaterial(materialName);

        if( mInstanceBatches.empty() )
            instanceBatch = BuildNewBatch( matname, true );
        else
            instanceBatch = GetFreeBatch( matname );

        return instanceBatch->CreateInstancedModel();
    }

    void DiInstanceManager::CleanupEmptyBatches( void )
    {
        UpdateDirtyBatches();

        auto itor = mInstanceBatches.begin();
        auto end  = mInstanceBatches.end();

        while( itor != end )
        {
            auto it = itor->second.begin();
            auto en = itor->second.end();

            while( it != en )
            {
                if( (*it)->IsBatchUnused() )
                {
                    it->reset();
                    size_t idx = it - itor->second.begin();
                    *it = itor->second.back();
                    itor->second.pop_back();

                    it = itor->second.begin() + idx;
                    en = itor->second.end();
                }
                else
                    ++it;
            }

            ++itor;
        }
    }

    void DiInstanceManager::DefragmentBatches(bool optimizeCull, DiVector<DiInstancedModelPtr> &usedEntities,
        InstanceBatchVec &fragmentedBatches)
    {
        auto itor = fragmentedBatches.begin();
        auto end  = fragmentedBatches.end();

        while( itor != end && !usedEntities.empty() )
        {
            if( !(*itor)->IsStatic() )
            {
                (*itor)->DefragmentBatch( optimizeCull, usedEntities );
            }
            ++itor;
        }

        InstanceBatchVec::iterator lastImportantBatch = itor;

        while( itor != end )
        {
            if( !(*itor)->IsStatic() )
            {
                (*itor)->DefragmentBatchDiscard();
                itor->reset();
            }
            else
            {
                *lastImportantBatch++ = *itor;
            }

            ++itor;
        }

        const size_t remainingBatches = end - lastImportantBatch;
        fragmentedBatches.resize( fragmentedBatches.size() - remainingBatches );
    }

    void DiInstanceManager::DefragmentBatches( bool optimizeCull )
    {
        UpdateDirtyBatches();

        InstanceBatchMap::iterator itor = mInstanceBatches.begin();
        InstanceBatchMap::iterator end  = mInstanceBatches.end();

        while( itor != end )
        {
            DiInstanceBatch::InstancedModelVec usedEntities;
            usedEntities.reserve( itor->second.size() * mInstancesPerBatch );

            InstanceBatchVec::iterator it = itor->second.begin();
            InstanceBatchVec::iterator en = itor->second.end();

            while( it != en )
            {
                if( !(*it)->IsStatic() )
                    (*it)->GetInstancedEntitiesInUse( usedEntities );
                ++it;
            }

            DefragmentBatches( optimizeCull, usedEntities, itor->second );

            ++itor;
        }
    }

    void DiInstanceManager::SetSetting( BatchSettingId id, bool value, const DiString& materialName /*= DiString::BLANK */ )
    {
        DI_ASSERT( id < NUM_SETTINGS );

        if( materialName == DiString::BLANK )
        {
            InstanceBatchMap::iterator itor = mInstanceBatches.begin();
            InstanceBatchMap::iterator end  = mInstanceBatches.end();

            while( itor != end )
            {
                mBatchSettings[itor->first].setting[id] = value;
                ApplySettingToBatches( id, value, itor->second );

                ++itor;
            }
        }
        else
        {
            mBatchSettings[materialName].setting[id] = value;

            InstanceBatchMap::const_iterator itor = mInstanceBatches.find( materialName );
            if( itor != mInstanceBatches.end() )
            {
                ApplySettingToBatches( id, value, itor->second );
            }
        }
    }

    bool DiInstanceManager::GetSetting( BatchSettingId id, const DiString &materialName ) const
    {
        DI_ASSERT( id < NUM_SETTINGS );

        BatchSettingsMap::const_iterator itor = mBatchSettings.find( materialName );
        if( itor != mBatchSettings.end() )
        {
            return itor->second.setting[id]; 
        }
        return BatchSettings().setting[id];
    }

    void DiInstanceManager::SetBatchesAsStaticAndUpdate( bool bStatic )
    {
        InstanceBatchMap::iterator itor = mInstanceBatches.begin();
        InstanceBatchMap::iterator end  = mInstanceBatches.end();

        while( itor != end )
        {
            InstanceBatchVec::iterator it = itor->second.begin();
            InstanceBatchVec::iterator en = itor->second.end();

            while( it != en )
            {
                (*it)->SetStaticAndUpdate( bStatic );
                ++it;
            }

            ++itor;
        }
    }

    void DiInstanceManager::AddDirtyBatch( DiInstanceBatchPtr dirtyBatch )
    {
        if( mDirtyBatches.empty() )
        {
            mSceneManager->DirtyInstanceManager( this );
        }

        mDirtyBatches.push_back( dirtyBatch );
    }

    void DiInstanceManager::UpdateDirtyBatches( void )
    {
        auto itor = mDirtyBatches.begin();
        auto end  = mDirtyBatches.end();

        while( itor != end )
        {
            (*itor)->UpdateBounds();
            ++itor;
        }

        mDirtyBatches.clear();
    }

    DiInstanceBatchPtr DiInstanceManager::GetFreeBatch(const DiString& materialName)
    {
        InstanceBatchVec &batchVec = mInstanceBatches[materialName];

        auto itor = batchVec.rbegin();
        auto end  = batchVec.rend();

        while( itor != end )
        {
            if( !(*itor)->IsBatchFull() )
            {
                return *itor;
            }
            ++itor;
        }

        return BuildNewBatch( materialName, false );
    }

    DiInstanceBatchPtr DiInstanceManager::BuildNewBatch(const DiString& materialName, bool firstTime)
    {
        DiSubMesh::IndexMap &idxMap = mMeshReference->GetSubMesh(mSubMeshIdx)->GetBlendIndexToBoneIndexMap();
        //idxMap = idxMap.empty() ? mMeshReference->sharedBlendIndexToBoneIndexMap : idxMap;

        DiMaterialPtr mat = DiAssetManager::GetInstance().GetAsset<DiMaterial>( materialName );

        InstanceBatchVec &materialInstanceBatch = mInstanceBatches[materialName];

        DiInstanceBatchPtr batch;

        DiString name;
        name.Format("%s_InstanceBatch_%d",mName.c_str(),mIdCount++);

        switch( mInstancingTechnique )
        {
        case INSTANCE_SHADER_BASED:
            batch = make_shared<DiInstanceBatchShader>( this, mMeshReference, mMotionReference, mat, mInstancesPerBatch,
                &idxMap, name );
            break;
        case INSTANCE_HARDWARE_BASED:
            batch = make_shared<DiInstanceBatchHardware>(this, mMeshReference, mat, mInstancesPerBatch,
                name );
            break;
        default:
            DI_ERROR("Unsupported instanced technique.");
            break;
        }

        if( !firstTime )
        {
            batch->BuildFrom( mMeshReference->GetSubMesh(mSubMeshIdx) );
        }
        else
        {
            const size_t maxInstPerBatch = batch->CalculateMaxNumInstances( mMeshReference->
                GetSubMesh(mSubMeshIdx) );
            mInstancesPerBatch = DiMath::Min( maxInstPerBatch, mInstancesPerBatch );
            batch->SetInstancesPerBatch( mInstancesPerBatch );

            batch->Build( mMeshReference->GetSubMesh(mSubMeshIdx) );
        }

        //const BatchSettings &batchSettings = mBatchSettings[materialName];
        //batch->SetCastShadows( batchSettings.setting[CAST_SHADOWS] );

        DiCullNode *sceneNode = mSceneManager->GetRootNode()->CreateChild();
        sceneNode->AttachObject( batch );

        materialInstanceBatch.push_back( batch );

        return batch;
    }

    void DiInstanceManager::ApplySettingToBatches( BatchSettingId id, bool value, const InstanceBatchVec &container )
    {
        InstanceBatchVec::const_iterator itor = container.begin();
        InstanceBatchVec::const_iterator end  = container.end();

        while( itor != end )
        {
            switch( id )
            {
            case CAST_SHADOWS:
                //(*itor)->SetCastShadows( value );
                break;
            default:
                break;
            }
            ++itor;
        }
    }

    DiString DiInstanceManager::SetupMaterial( const DiString& materialName )
    {
        DiString insmaterial = materialName + ".instance";

        DiMaterialPtr nmt = DiAssetManager::GetInstance().FindAsset<DiMaterial>(insmaterial);
        if ( nmt )
        {
            return insmaterial;
        }
        else
        {
            DiMaterialPtr mt = DiAssetManager::GetInstance().GetAsset<DiMaterial>(materialName);
            DiMaterialPtr newmt = mt->Clone(insmaterial);
            newmt->SetInstanceType(mInstancingTechnique);
            return insmaterial;
        }
    }
}