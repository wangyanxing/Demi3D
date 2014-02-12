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

#ifndef DiInstanceManager_h__
#define DiInstanceManager_h__


#include "Mesh.h"
#include "Motion.h"
#include "InstancedModel.h"
#include "InstanceBatch.h"

namespace Demi
{
    class DI_GFX_API DiInstanceManager
    {
    public:

        friend class DiInstanceBatch;

        enum BatchSettingId
        {
            CAST_SHADOWS        = 0,
            SHOW_BOUNDINGBOX,

            NUM_SETTINGS
        };

        struct BatchSettings
        {
            bool    setting[NUM_SETTINGS];

            BatchSettings()
            {
                setting[CAST_SHADOWS]                = true;
                setting[SHOW_BOUNDINGBOX]            = false;
            }
        };

        DiInstanceManager(DiSceneManager* sm, const DiString& idname, const DiString& meshName,
            DiInstanceType insTech,uint32 inssPerBatch);

        DiInstanceManager(DiSceneManager* sm, const DiString& idname, const DiString& meshName,const DiString& motionName,
            DiInstanceType insTech,uint32 inssPerBatch);

        virtual                 ~DiInstanceManager();

    public:

        typedef DiVector<DiInstanceBatchPtr>        InstanceBatchVec;    //vec[batchN] = Batch

        typedef DiMap<DiString, InstanceBatchVec>    InstanceBatchMap;    //map[materialName] = Vec

        typedef DiMap<DiString, BatchSettings>        BatchSettingsMap;

    public:

        DiString                GetName() const { return mName; }

        void                    SetInstancesPerBatch( size_t instancesPerBatch );

        void                    SetMaxLookupTableInstances( size_t maxLookupTableInstances );

        size_t                  GetMaxOrBestNumInstancesPerBatch( const DiString& materialName,
                                  size_t suggestedSize, uint16 flags );

        DiInstancedModelPtr     CreateInstancedModel(const DiString& materialName);

        void                    CleanupEmptyBatches(void);

        void                    DefragmentBatches( bool optimizeCulling );

        void                    SetSetting( BatchSettingId id, bool value, const DiString& materialName = DiString::BLANK );

        bool                    GetSetting( BatchSettingId id, const DiString &materialName ) const;

        bool                    HasSettings( const DiString &materialName )
                                    { return mBatchSettings.contains( materialName ); }

        void                    SetBatchesAsStaticAndUpdate( bool bStatic );

        void                    AddDirtyBatch(DiInstanceBatchPtr dirtyBatch);

        void                    UpdateDirtyBatches(void);

        typedef DiConstMapIterator<InstanceBatchMap>    InstanceBatchMapIterator;
        typedef DiConstVectorIterator<InstanceBatchVec> InstanceBatchIterator;

        InstanceBatchMapIterator GetInstanceBatchMapIterator(void) const
                                        { return InstanceBatchMapIterator( mInstanceBatches.begin(), mInstanceBatches.end() ); }

        InstanceBatchIterator    GetInstanceBatchIterator( const DiString &materialName ) const
        {
            InstanceBatchMap::const_iterator it = mInstanceBatches.find( materialName );
            return InstanceBatchIterator( it->second.begin(), it->second.end() );
        }

    protected:

        DiInstanceBatchPtr      GetFreeBatch( const DiString& materialName );

        DiInstanceBatchPtr      BuildNewBatch(const DiString& materialName, bool firstTime);

        void                    DefragmentBatches(bool optimizeCull, DiVector<DiInstancedModelPtr>& models,
                                    InstanceBatchVec &fragmentedBatches );

        void                    ApplySettingToBatches( BatchSettingId id, bool value, const InstanceBatchVec &container );

        DiString                SetupMaterial(const DiString& name);

    protected:

        DiString                mName;

        DiMeshPtr               mMeshReference;

        DiMotionPtr             mMotionReference;
        
        InstanceBatchMap        mInstanceBatches;
        
        size_t                  mIdCount;

        InstanceBatchVec        mDirtyBatches;

        size_t                  mInstancesPerBatch;
        
        DiInstanceType          mInstancingTechnique;

        uint16                  mSubMeshIdx;

        BatchSettingsMap        mBatchSettings;
        
        DiSceneManager*         mSceneManager;

        size_t                  mMaxLookupTableInstances;
    };
}

#endif
