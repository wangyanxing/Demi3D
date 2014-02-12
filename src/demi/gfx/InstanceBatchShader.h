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

#ifndef DiInstanceBatchShader_h__
#define DiInstanceBatchShader_h__


#include "GfxPrerequisites.h"
#include "InstanceBatch.h"

namespace Demi
{
    class DI_GFX_API DiInstanceBatchShader : public DiInstanceBatch
    {
    public:
        DiInstanceBatchShader( DiInstanceManager *creator, DiMeshPtr &meshReference,
                            DiMotionPtr &motionReference, const DiMaterialPtr &material,
                            size_t instancesPerBatch, const DiSubMesh::IndexMap *indexToBoneMap,
                            const DiString &batchName );

        virtual ~DiInstanceBatchShader();

    public:

        uint32  CalculateMaxNumInstances( const DiSubMesh* baseSubMesh ) const;

        void    BuildFrom( const DiSubMesh* baseSubMesh );
                
        void    GetWorldTransform( DiMat4* xform ) const;
                
        uint32  GetWorldTransformNums(void) const;

    protected:

        uint32  m_usNumWorldMatrices;

        void    SetupVertices( const DiSubMesh* baseSubMesh );
                
        void    SetupIndices( const DiSubMesh* baseSubMesh );
                
        void    SetupHardwareSkinned( const DiSubMesh* baseSubMesh );
                
        void    SetupNoSkinned( const DiSubMesh* baseSubMesh );

    };
}

#endif
