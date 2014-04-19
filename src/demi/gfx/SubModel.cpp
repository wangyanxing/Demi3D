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
#include "SubModel.h"
#include "Model.h"
#include "VertexBuffer.h"
#include "VertexDeclaration.h"
#include "IndexBuffer.h"
#include "GfxDriver.h"

namespace Demi 
{
    DiSubModel::DiSubModel(DiModel* parent, DiSubMesh* model, bool softwareSkin)
        :mParent(parent),
        mMesh(model),
        mLocalWeights(nullptr),
        mLocalIndices(nullptr),
        mSoftBlendData(nullptr),
        mBoneTransforms(nullptr)
    {
        InitFromSubMesh(softwareSkin);
    }

    DiSubModel::~DiSubModel()
    {
        ReleaseSourceData();
        ReleaseIndexBuffer();
        ReleaseVertexDeclaration();

        SAFE_ARRAY_DELETE(mLocalIndices);
        SAFE_ARRAY_DELETE(mLocalWeights);
        SAFE_DELETE(mSoftBlendData);
    }

    void DiSubModel::GetWorldTransform( DiMat4* xform ) const
    {
        *xform = mParent->GetTransform();
    }

#define use_btvec 0

    void DiSubModel::SoftwareVertBlend()
    {
        DI_ASSERT(mSoftBlendData);
        DI_ASSERT(mSoftBlendData->hasPos);
        
        DiVertexBuffer* vb = mSourceData[0];
        DiVec3* buf = (DiVec3*)vb->Lock(0, vb->GetBufferSize());
        float* sourceBuf = mSoftBlendData->data;
        
        btVector3 src;
        src.setZero();
        btVector3 accu,t;
        
        for (size_t v = 0; v < mVerticesNum; ++v)
        {
            *buf = DiVec3::ZERO;
            src.setValue(sourceBuf[0], sourceBuf[1], sourceBuf[2]);
            for (size_t w = 0; w < mLocalIndices[v].size(); ++w)
            {
                float weight = mLocalWeights[v][w];
#if use_btvec
                const btTransform& mat = mBoneTransforms[mLocalIndices[v][w]];
                t = mat.getBasis() * src + mat.getOrigin();
                t *= weight;
                buf->x += t.getX();
                buf->y += t.getY();
                buf->z += t.getZ();
#else
                const DiMat4& mat = mBoneMatrices[mLocalIndices[v][w]];
                buf->x +=
                    (mat[0][0] * sourceBuf[0] +
                    mat[0][1] * sourceBuf[1] +
                    mat[0][2] * sourceBuf[2] +
                    mat[0][3]) * weight;

                buf->y +=
                    (mat[1][0] * sourceBuf[0] +
                    mat[1][1] * sourceBuf[1] +
                    mat[1][2] * sourceBuf[2] +
                    mat[1][3]) * weight;

                buf->z +=
                    (mat[2][0] * sourceBuf[0] +
                    mat[2][1] * sourceBuf[1] +
                    mat[2][2] * sourceBuf[2] +
                    mat[2][3]) * weight;
#endif
            }
            sourceBuf += 3;
            buf++;
            
            if (mSoftBlendData->hasNormal)
            {
                *buf = DiVec3::ZERO;
#if use_btvec
                accu.setZero();
#endif
                src.setValue(sourceBuf[0], sourceBuf[1], sourceBuf[2]);
                for (size_t w = 0; w < mLocalIndices[v].size(); ++w)
                {
                    float weight = mLocalWeights[v][w];
#if use_btvec
                    const btTransform& mat = mBoneTransforms[mLocalIndices[v][w]];
                    accu += mat.getBasis() * src * weight;
#else
                    const DiMat4& mat = mBoneMatrices[mLocalIndices[v][w]];
                    buf->x +=
                    (mat[0][0] * sourceBuf[0] +
                     mat[0][1] * sourceBuf[1] +
                     mat[0][2] * sourceBuf[2]) * weight;
                    
                    buf->y +=
                    (mat[1][0] * sourceBuf[0] +
                     mat[1][1] * sourceBuf[1] +
                     mat[1][2] * sourceBuf[2]) * weight;
                    
                    buf->z +=
                    (mat[2][0] * sourceBuf[0] +
                     mat[2][1] * sourceBuf[1] +
                     mat[2][2] * sourceBuf[2]) * weight;
#endif
                }
                sourceBuf += 3;
#if use_btvec
                accu.normalize();
                buf->x = accu.getX();
                buf->y = accu.getY();
                buf->z = accu.getZ();
#else
                buf->normalise();
#endif
                
                buf++;
            }
            if (mSoftBlendData->hasTangent)
            {
                *buf = DiVec3::ZERO;
#if use_btvec
                accu.setZero();
#endif
                src.setValue(sourceBuf[0], sourceBuf[1], sourceBuf[2]);
                for (size_t w = 0; w < mLocalIndices[v].size(); ++w)
                {
                    float weight = mLocalWeights[v][w];
#if use_btvec
                    const btTransform& mat = mBoneTransforms[mLocalIndices[v][w]];
                    accu += mat.getBasis() * src * weight;
#else
                    const DiMat4& mat = mBoneMatrices[mLocalIndices[v][w]];
                    buf->x += 
                    (mat[0][0] * sourceBuf[0] +
                     mat[0][1] * sourceBuf[1] +
                     mat[0][2] * sourceBuf[2]) * weight;
                    
                    buf->y +=
                    (mat[1][0] * sourceBuf[0] +
                     mat[1][1] * sourceBuf[1] +
                     mat[1][2] * sourceBuf[2]) * weight;
                    
                    buf->z +=
                    (mat[2][0] * sourceBuf[0] +
                     mat[2][1] * sourceBuf[1] +
                     mat[2][2] * sourceBuf[2]) * weight;
#endif
                }
                sourceBuf += 3;
#if use_btvec
                accu.normalize();
                buf->x = accu.getX();
                buf->y = accu.getY();
                buf->z = accu.getZ();
#else
                buf->normalise();
#endif
                buf++;
            }
        }
        vb->Unlock();
    }
    
    void DiSubModel::InitFromSubMesh(bool softwareSkin)
    {
        ReleaseSourceData();
        ReleaseIndexBuffer();
        ReleaseVertexDeclaration();
        
        mVertexDecl = Driver->CreateVertexDeclaration();
        bool useOriginalVertDecl = mMesh->mVFElements == 0 || !softwareSkin;
        if (!useOriginalVertDecl)
            mVertexDecl->AddElements(mMesh->mVFElements, softwareSkin);
        else
            mVertexDecl->AddElements(mMesh->mVertexElems);
        mVertexDecl->Create();
        
        if (mMesh->mIndexData && mMesh->GetIndexNum() > 0)
        {
            mIndexBuffer = Driver->CreateIndexBuffer();

            uint32 size = mMesh->GetIndexSize() * mMesh->GetIndexNum();
            mIndexBuffer->Create(size,
                mMesh->m32BitIndex ? IB_32BITS : IB_16BITS,
                RU_WRITE_ONLY);

            mIndexBuffer->WriteData(0, size, mMesh->mIndexData);

            mPrimitiveCount = mMesh->GetPrimitiveCount();
            mPrimitiveType  = mMesh->GetPrimitiveType();
        }
        
        mVerticesNum = mMesh->GetVerticeNum();
        
        // setup software skinning data
        if (!mParent->UseHardwareSkinning())
        {
            DI_ASSERT(!mLocalIndices);
            DI_ASSERT(!mLocalWeights);
            
            mLocalWeights = DI_NEW FastArray<float>[mVerticesNum];
            mLocalIndices = DI_NEW FastArray<uint8>[mVerticesNum];
            
            auto i = mMesh->mBoneWeights.begin();
            auto iend = mMesh->mBoneWeights.end();
            for (size_t v = 0; v < mVerticesNum; ++v)
            {
                mLocalWeights[v].resize(mMesh->mMaxWeights);
                mLocalIndices[v].resize(mMesh->mMaxWeights);
                
                for (uint16 bone = 0; bone < mMesh->mMaxWeights; ++bone)
                {
                    if (i != iend && i->second.vertexIndex == v)
                    {
                        mLocalWeights[v][bone] = i->second.weight;
                        mLocalIndices[v][bone] = static_cast<uint8>(i->second.boneIndex);
                        ++i;
                    }
                    else
                    {
                        mLocalWeights[v][bone] = (bone == 0) ? 1.0f : 0.0f;
                        mLocalIndices[v][bone] = 0;
                    }
                }
            }
            
            mSoftBlendData = mMesh->GenerateBlendData();
        }
       
        if (useOriginalVertDecl)
        {
            // just use the orignal data
            for (auto it = mMesh->mVertexData.begin();
                 it != mMesh->mVertexData.end(); ++it)
            {
                DiVertexBuffer* buf = Driver->CreateVertexBuffer();
                uint32 size = it->second.GetSize();
                buf->SetStride(it->second.stride);
                buf->Create(size, RU_WRITE_ONLY, it->first);
                buf->WriteData(0, size, it->second.data);
                mSourceData.push_back(buf);
            }
        }
        else
        {
            auto& elements = mVertexDecl->GetElements();
            DiVector<DiVertexElements::Element> mappings;
            // map two vertex declarations
            for (size_t e = 0; e < elements.mVertexElements.size(); ++e)
            {
                auto ele = mMesh->mVertexElems.FindElement((DiVertexType)elements.mVertexElements[e].Type,
                                                (DiVertexUsage)elements.mVertexElements[e].Usage,
                                                elements.mVertexElements[e].UsageIndex);
                mappings.push_back(ele);
            }
            
            // reorder
            uint16 streamNum = elements.GetStreams();
            uint16 startStream = softwareSkin ? 1 : 0;
            if(softwareSkin)
            {
                // deal with first stream
                DiVertexBuffer* buf = Driver->CreateVertexBuffer();
                uint16 stride = elements.GetStreamElementsSize(0);
                buf->SetStride(stride);
                uint32 size = stride * mVerticesNum;
                buf->Create(size, RU_WRITE_ONLY, 0);
                buf->WriteData(0, size, mSoftBlendData->data);
                mSourceData.push_back(buf);
            }
            
            for (uint16 i = startStream; i < streamNum; ++i)
            {
                DiVertexBuffer* buf = Driver->CreateVertexBuffer();
                uint16 stride = elements.GetStreamElementsSize(i);
                buf->SetStride(stride);
                uint32 size = stride * mVerticesNum;

                buf->Create(size, RU_WRITE_ONLY,i);
                uint8* vb = (uint8*)buf->Lock(0, size);
                for (uint32 v = 0; v < mVerticesNum; ++v)
                {
                    for (size_t e = 0; e < elements.mVertexElements.size(); ++e)
                    {
                        if(elements.mVertexElements[e].Stream != i)
                            continue;
                        auto sd = mMesh->mVertexData[mappings[e].Stream];
                        uint8* srcMem = (uint8*)sd.data + v*sd.stride + mappings[e].Offset;
                        uint8* dstMem = vb + v*stride + elements.mVertexElements[e].Offset;
                        uint16 typesize = DiVertexElements::GetElementTypeSize((DiVertexType)elements.mVertexElements[e].Type);
                        memcpy(dstMem,srcMem,typesize);
                    }
                }
                buf->Unlock();
                mSourceData.push_back(buf);
            }
        }
        
        SetMaterial(mMesh->GetMaterialName());
    }
}