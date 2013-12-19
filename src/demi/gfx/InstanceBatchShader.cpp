
#include "InstanceBatchShader.h"
#include "InstancedModel.h"
#include "VertexDeclaration.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "GfxDriver.h"

#pragma warning(disable:4244)

namespace Demi
{
    DiInstanceBatchShader::DiInstanceBatchShader( DiInstanceManager *creator, DiMeshPtr &meshReference,
        DiMotionPtr& motionReference,const DiMaterialPtr &material, size_t instancesPerBatch,
        const DiSubMesh::IndexMap *indexToBoneMap, const DiString &batchName ) :
        DiInstanceBatch( creator, meshReference, motionReference, material, instancesPerBatch,
        indexToBoneMap, batchName ),
        m_usNumWorldMatrices( instancesPerBatch )
    {
    }

    DiInstanceBatchShader::~DiInstanceBatchShader()
    {
    }
    
    uint32 DiInstanceBatchShader::CalculateMaxNumInstances( const DiSubMesh *baseSubMesh ) const
    {
        uint32 numBones = 1;
        if (HasSkeleton())
        {
            numBones = DiMath::Max( 1, mMotionReference->GetSkeleton()->GetNumBones());
        }

        return MAX_BONE_NUM / numBones;
    }
    
    void DiInstanceBatchShader::BuildFrom( const DiSubMesh *baseSubMesh )
    {
        if( HasSkeleton() )
        {
            m_usNumWorldMatrices = mInstancesPerBatch * baseSubMesh->GetBlendIndexToBoneIndexMap().size();
        }
        DiInstanceBatch::BuildFrom( baseSubMesh );
    }
    
    void DiInstanceBatchShader::SetupVertices( const DiSubMesh* baseSubMesh )
    {
        mRemoveOwnVertexData = true;

        mVerticesNum = baseSubMesh->GetVerticeNum() * mInstancesPerBatch;

        if( HasSkeleton() )
        {
            SetupHardwareSkinned( baseSubMesh );
        }
        else
        {
            SetupNoSkinned( baseSubMesh );
        }
    }
    
    void DiInstanceBatchShader::SetupIndices( const DiSubMesh* baseSubMesh )
    {
        mIndexBuffer = Driver->CreateIndexBuffer();
        mRemoveOwnIndexData = true;

        bool use32BitId = mVerticesNum > 65535;

        uint32 size = baseSubMesh->GetIndexSize()*baseSubMesh->GetIndexNum()*mInstancesPerBatch;
        mIndexBuffer->Create(size, use32BitId ? IB_32BITS : IB_16BITS, RU_WRITE_ONLY);

        void* data = mIndexBuffer->Lock(0,size);

        uint16 *thisBuf16 = static_cast<uint16*>(data);
        uint32 *thisBuf32 = static_cast<uint32*>(data);

        for (uint32 i = 0; i < mInstancesPerBatch; i++)
        {
            const size_t vertexOffset = i * mVerticesNum / mInstancesPerBatch;

            uint16 const *initBuf16 = static_cast<uint16 const *>(baseSubMesh->GetIndexData());
            uint32 const *initBuf32 = static_cast<uint32 const *>(baseSubMesh->GetIndexData());

            for( size_t j=0; j<baseSubMesh->GetIndexNum(); ++j )
            {
                uint32 originalVal;
                if( baseSubMesh->GetUse32BitIndex() )
                {
                    originalVal = *initBuf32++;
                }
                else
                {
                    originalVal = *initBuf16++;
                }

                if( !use32BitId )
                {
                    *thisBuf16++ = static_cast<uint16>(originalVal) + vertexOffset;
                }
                else
                {
                    *thisBuf32++ = originalVal + vertexOffset;
                }
            }
        }
        mIndexBuffer->Unlock();

        mPrimitiveCount = baseSubMesh->GetPrimitiveCount() * mInstancesPerBatch;
        mPrimitiveType  = baseSubMesh->GetPrimitiveType();
    }
    
    void DiInstanceBatchShader::GetWorldTransform( DiMat4* xform ) const
    {
        InstancedModelVec::const_iterator itor = mInstancedModels.begin();
        InstancedModelVec::const_iterator end  = mInstancedModels.end();

        while( itor != end )
        {
            xform += (*itor)->GetTransforms( xform );
            ++itor;
        }
    }
    
    uint32 DiInstanceBatchShader::GetWorldTransformNums(void) const
    {
        return m_usNumWorldMatrices;
    }

    void DiInstanceBatchShader::SetupNoSkinned( const DiSubMesh* baseSubMesh )
    {
        mVertexDecl = Driver->CreateVertexDeclaration();
        mVertexDecl->AddElements(baseSubMesh->GetVertexElements());
        mVertexDecl->AddElement(mVertexDecl->GetElements().GetStreams(),VERT_TYPE_UBYTE4,VERT_USAGE_BLENDINDICES,0);
        mVertexDecl->Create();

        DiSubMesh::SourceDataIterator si = baseSubMesh->GetSourceDataIterator();
        while (si.HasMoreElements())
        {
            DiSubMesh::SourceData sd = si.GetNext();

            DiVertexBuffer* buf = Driver->CreateVertexBuffer();
            uint32 size = sd.GetSize() * mInstancesPerBatch;
            buf->SetStride(sd.stride);
            buf->Create(size,RU_WRITE_ONLY,sd.stream);
            BYTE* data = (BYTE*)buf->Lock(0,size);
            for( size_t j=0; j < mInstancesPerBatch; ++j )
            {
                memcpy(data + sd.GetSize()*j,sd.data,sd.GetSize());
            }
            buf->Unlock();
            mSourceData.push_back(buf);
        }

        uint16 lastStream = mVertexDecl->GetElements().GetStreams()-1;
        DiVertexBuffer* buf = Driver->CreateVertexBuffer();
        uint32 stride = mVertexDecl->GetElements().GetStreamElementsSize(lastStream);
        uint32 size = baseSubMesh->GetVerticeNum() * stride * mInstancesPerBatch;
        buf->SetStride(stride);
        buf->Create(size, RU_WRITE_ONLY, lastStream);
        char* thisBuf = static_cast<char*>(buf->Lock(0,size));
        for( size_t j=0; j < mInstancesPerBatch; ++j )
        {
            for( size_t k=0; k<baseSubMesh->GetVerticeNum(); ++k )
            {
                *thisBuf++ = mInstancesPerBatch - j - 1;
                *thisBuf++ = mInstancesPerBatch - j - 1;
                *thisBuf++ = mInstancesPerBatch - j - 1;
                *thisBuf++ = mInstancesPerBatch - j - 1;
            }
        }
        buf->Unlock();
        mSourceData.push_back(buf);
    }

    void DiInstanceBatchShader::SetupHardwareSkinned( const DiSubMesh* baseSubMesh )
    {
        const size_t numBones = mMotionReference->GetSkeleton()->GetNumBones();//baseSubMesh->GetBlendIndexToBoneIndexMap().size();
        m_usNumWorldMatrices = mInstancesPerBatch * numBones;


        DiVertexElements eles = baseSubMesh->GetVertexElements();

        uint16 nextFreeUV = eles.GetNextFreeTexcoord();
        if (nextFreeUV > 8-1)
        {
            DI_WARNING("No enough rooms of texcoord");
            return;
        }

        uint16 blendstream = eles.GetTypeUsageAtStream(VERT_TYPE_UBYTE4,VERT_USAGE_BLENDINDICES);
        eles.AddElement(blendstream, VERT_TYPE_FLOAT1, VERT_USAGE_TEXCOORD, 7);    //instanceID
        mVertexDecl = Driver->CreateVertexDeclaration();
        mVertexDecl->AddElements(eles);
        mVertexDecl->Create();

        DiSubMesh::SourceDataIterator si = baseSubMesh->GetSourceDataIterator();
        while (si.HasMoreElements())
        {
            DiSubMesh::SourceData sd = si.GetNext();

            DiVertexElements& ves = const_cast<DiSubMesh*>(baseSubMesh)->GetVertexElements();
            DiVertexElements::ElementsList elementlist = ves.GetElementsAtStream(sd.stream);

            DiVertexBuffer* buf = Driver->CreateVertexBuffer();
            uint32 size = sd.GetSize() * mInstancesPerBatch;
            buf->SetStride(sd.stride);
            buf->Create(size, RU_WRITE_ONLY, sd.stream);
            BYTE* startBuf = (BYTE*)buf->Lock(0,size);
            BYTE* data = startBuf;
            BYTE* startSrcBuf = (BYTE*)sd.data;
            BYTE* dataSrc = startSrcBuf;

            for( size_t j=0; j < mInstancesPerBatch; ++j )
            {
                dataSrc = startSrcBuf;
                for (uint32 v = 0; v < sd.numVertices; v++)
                {
                    for (size_t ei = 0; ei < elementlist.size(); ei++)
                    {
                        if (elementlist[ei].Usage == VERT_USAGE_BLENDINDICES)
                        {
                            *(data + elementlist[ei].Offset + 0) = *(dataSrc + elementlist[ei].Offset + 0) + j * numBones;
                            *(data + elementlist[ei].Offset + 1) = *(dataSrc + elementlist[ei].Offset + 1) + j * numBones;
                            *(data + elementlist[ei].Offset + 2) = *(dataSrc + elementlist[ei].Offset + 2) + j * numBones;
                            *(data + elementlist[ei].Offset + 3) = *(dataSrc + elementlist[ei].Offset + 3) + j * numBones;
                        }
                        else if (elementlist[ei].Usage == VERT_USAGE_TEXCOORD && elementlist[ei].UsageIndex == 7)
                        {
                            float* d = (float*)(data + elementlist[ei].Offset);
                            *d = (float)(j*numBones);
                        }
                        else
                        {
                            memcpy(data + elementlist[ei].Offset,dataSrc + elementlist[ei].Offset,
                                DiVertexElements::GetElementTypeSize(DiVertexType(elementlist[ei].Type)));
                        }
                    }
                    data    += const_cast<DiSubMesh*>(baseSubMesh)->GetVertexElements().GetStreamElementsSize(sd.stream);
                    dataSrc += const_cast<DiSubMesh*>(baseSubMesh)->GetVertexElements().GetStreamElementsSize(sd.stream);
                }
            }
            buf->Unlock();
            mSourceData.push_back(buf);
        }
    }
}