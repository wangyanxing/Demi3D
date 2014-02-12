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

#ifndef DiInstanceBatchHardware_h__
#define DiInstanceBatchHardware_h__


#include "InstanceBatch.h"

namespace Demi
{
    class DI_GFX_API DiInstanceBatchHardware : public DiInstanceBatch
    {
    public:

        DiInstanceBatchHardware( DiInstanceManager*creator, 
            DiMeshPtr &meshReference,
            const DiMaterialPtr &material,
            size_t instancesPerBatch, 
            const DiString &batchName );

        ~DiInstanceBatchHardware();

    public:

        uint32          CalculateMaxNumInstances( const DiSubMesh *baseSubMesh ) const;

        void            BuildFrom( const DiSubMesh *baseSubMesh );

        void            BoundsDirty(void);

        void            SetStaticAndUpdate( bool bStatic );

        bool            IsStatic() const { return mKeepStatic; }

        void            GetWorldTransform( DiMat4* xform ) const;

        void            CullingUpdate(DiRenderBatchGroup*,DiCamera*);

        void            AddToBatchGroup(DiRenderBatchGroup* bg);

        void            Update(DiCamera* camera);

    protected:

        void            SetupVertices( const DiSubMesh* baseSubMesh );
        
        void            SetupIndices( const DiSubMesh* baseSubMesh );

        int             RemoveBlendData(DiVertexElements& elements);

        virtual bool    CheckSubMeshCompatibility( const DiSubMesh* baseSubMesh );

        uint32          UpdateVertexBuffer( DiCamera *currentCamera );

    protected:

        bool            mKeepStatic;

        bool            mAddToBatch;
    };
}

#endif
