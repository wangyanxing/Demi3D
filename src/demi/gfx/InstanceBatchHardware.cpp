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
#include "InstanceBatchHardware.h"
#include "InstanceManager.h"
#include "VertexBuffer.h"
#include "VertexDeclaration.h"
#include "IndexBuffer.h"
#include "InstancedModel.h"
#include "RenderBatchGroup.h"
#include "GfxDriver.h"

namespace Demi
{
    DiInstanceBatchHardware::DiInstanceBatchHardware( DiInstanceManager*creator,
        DiMeshPtr &meshReference, const DiMaterialPtr &material, size_t instancesPerBatch, const DiString &batchName ):
        DiInstanceBatch( creator, meshReference, DiMotionPtr(),material, instancesPerBatch,
        NULL, batchName ),
        mAddToBatch(false)
    {
        mTechnSupportsSkeletal = false;
        mKeepStatic = false;
    }

    DiInstanceBatchHardware::~DiInstanceBatchHardware()
    {

    }

    uint32 DiInstanceBatchHardware::CalculateMaxNumInstances( const DiSubMesh *baseSubMesh ) const
    {
        uint32 retVal = 0;
        if (/*DiDx9Capability::CapGetValue(DX9CAP_HADWARE_INSTANCE) ==*/ true)
        {
            retVal = 65535;
        }

        return retVal;
    }

    void DiInstanceBatchHardware::BuildFrom( const DiSubMesh *baseSubMesh )
    {
        DiInstanceBatch::BuildFrom(baseSubMesh);
    }

    void DiInstanceBatchHardware::BoundsDirty( void )
    {
        if( !mBoundsDirty && !mKeepStatic )
        {
            mCreator->AddDirtyBatch(std::dynamic_pointer_cast<DiInstanceBatch>(shared_from_this()));
        }
        mBoundsDirty = true;
    }

    void DiInstanceBatchHardware::SetStaticAndUpdate( bool bStatic )
    {
        if( mKeepStatic && mBoundsDirty )
        {
            mCreator->AddDirtyBatch(std::dynamic_pointer_cast<DiInstanceBatch>(shared_from_this()));
        }

        mKeepStatic = bStatic;
        if( mKeepStatic )
        {
            UpdateVertexBuffer( NULL );
        }
    }

    void DiInstanceBatchHardware::GetWorldTransform( DiMat4* xform ) const
    {
        *xform = DiMat4::IDENTITY;
    }

    void DiInstanceBatchHardware::SetupVertices( const DiSubMesh* baseSubMesh )
    {
        mRemoveOwnVertexData = true;    

        mVerticesNum = baseSubMesh->GetVerticeNum();

        DiVertexElements elements = baseSubMesh->GetVertexElements();
        int blendSource = RemoveBlendData(elements);

        mVertexDecl = Driver->CreateVertexDeclaration();
        mVertexDecl->AddElements(elements);
        uint16 nextFreeStream = 0;
        
        if (blendSource != -1)
            nextFreeStream = (uint16)blendSource;
        else
            nextFreeStream = mVertexDecl->GetElements().GetStreams();

        uint16 nextFreeUV = mVertexDecl->GetElements().GetNextFreeTexcoord();
        if (nextFreeUV > 8-2)
        {
            DI_WARNING("No enough rooms for texcoords");
            return;
        }

        mVertexDecl->AddElement(nextFreeStream, VERT_TYPE_FLOAT4, VERT_USAGE_TEXCOORD,5);
        mVertexDecl->AddElement(nextFreeStream, VERT_TYPE_FLOAT4, VERT_USAGE_TEXCOORD,6);
        mVertexDecl->AddElement(nextFreeStream, VERT_TYPE_FLOAT4, VERT_USAGE_TEXCOORD,7);
        mVertexDecl->Create();

        DiSubMesh::SourceDataIterator si = baseSubMesh->GetSourceDataIterator();
        while (si.HasMoreElements())
        {
            DiSubMesh::SourceData sd = si.GetNext();

            if ((int)(sd.stream) == blendSource)
            {
                continue;
            }

            DiVertexBuffer* buf = Driver->CreateVertexBuffer();
            uint32 size = sd.GetSize();
            buf->SetStride(sd.stride);
            buf->Create(size,RU_WRITE_ONLY,sd.stream);
            BYTE* data = (BYTE*)buf->Lock(0,size);
            memcpy(data,sd.data,sd.GetSize());
            buf->Unlock();
            buf->SetInstanceState(SOURCE_INSTANCE_INDEX);
            buf->SetInstanceNum(mInstancesPerBatch);
            mSourceData.push_back(buf);
        }

        DiVertexBuffer* buf = Driver->CreateVertexBuffer();
        uint32 stride = mVertexDecl->GetElements().GetStreamElementsSize(nextFreeStream);
        uint32 size = mInstancesPerBatch * stride ;
        buf->SetStride(stride);
        buf->Create(size, RU_WRITE_ONLY, nextFreeStream);
        buf->SetInstanceState(SOURCE_INSTANCE_DATA);
        mSourceData.push_back(buf);
    }

    void DiInstanceBatchHardware::SetupIndices( const DiSubMesh* baseSubMesh )
    {
        mRemoveOwnIndexData = true;

        if (baseSubMesh->GetIndexData() && baseSubMesh->GetIndexNum() > 0)
        {
            mIndexBuffer = Driver->CreateIndexBuffer();

            uint32 size = baseSubMesh->GetIndexSize()*baseSubMesh->GetIndexNum();
            mIndexBuffer->Create(size,
                baseSubMesh->GetUse32BitIndex() ? IB_32BITS : IB_16BITS,
                RU_WRITE_ONLY);

            void* data = mIndexBuffer->Lock(0,size);
            memcpy(data,baseSubMesh->GetIndexData(),size);
            mIndexBuffer->Unlock();

            mPrimitiveCount = baseSubMesh->GetPrimitiveCount();
            mPrimitiveType  = baseSubMesh->GetPrimitiveType();
        }

    }

    int DiInstanceBatchHardware::RemoveBlendData(DiVertexElements& elements)
    {
        bool hasBlendid = elements.Contains(VERT_USAGE_BLENDINDICES);
        bool hasBlendWt = elements.Contains(VERT_USAGE_BLENDWEIGHT);
        if(!hasBlendid && !hasBlendWt)
        {
            return -1;
        }

        uint16 blendIDSrc = 0;
        if (hasBlendid)
        {
            blendIDSrc = elements.GetUsageAtStream(VERT_USAGE_BLENDINDICES);
        }
        uint16 blendWtSrc = 0;
        if (hasBlendWt)
        {
            blendWtSrc = elements.GetUsageAtStream(VERT_USAGE_BLENDWEIGHT);
        }

        uint16 srcToDel = 0;
        if (hasBlendWt && hasBlendid)
        {
            DI_ASSERT(blendIDSrc==blendWtSrc);
            srcToDel = blendIDSrc;
        }
        else
        {
            srcToDel = hasBlendid?blendIDSrc:blendWtSrc;
        }

        elements.DeleteSource(srcToDel);
        return (int)srcToDel;
    }

    bool DiInstanceBatchHardware::CheckSubMeshCompatibility( const DiSubMesh* baseSubMesh )
    {
        return true;
    }

    uint32 DiInstanceBatchHardware::UpdateVertexBuffer( DiCamera *currentCamera )
    {
        size_t retVal = 0;
        DiVertexBuffer* vb = mSourceData.back();
        float *pDest = static_cast<float*>(vb->Lock(0,vb->GetBufferSize()));

        InstancedModelVec::const_iterator itor = mInstancedModels.begin();
        InstancedModelVec::const_iterator end  = mInstancedModels.end();

        while( itor != end )
        {
            if( (*itor)->IsInUse()/* && (*itor)->FindVisible( currentCamera )*/ )
            {
                const size_t floatsWritten = (*itor)->GetTransforms3x4( pDest );

                pDest += floatsWritten;

                ++retVal;
            }
            ++itor;
        }

        vb->Unlock();

        DI_ASSERT(mSourceData.size() > 1);
        for (size_t i=0; i < mSourceData.size()-1; i++)
        {
            mSourceData[i]->SetInstanceNum(retVal);
        }

        return retVal;
    }

    void DiInstanceBatchHardware::AddToBatchGroup(DiRenderBatchGroup* bg)
    {
        if (mAddToBatch)
        {
            bg->AddRenderUnit(this);
            mAddToBatch = false;
        }
    }

    void DiInstanceBatchHardware::Update(DiCamera* camera)
    {
        if (!mKeepStatic)
            mAddToBatch = UpdateVertexBuffer(NULL) != 0;
        else
            mAddToBatch = true;
    }
}