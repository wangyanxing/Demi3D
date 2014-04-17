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
        mSoftBlendData(nullptr)
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

    void DiSubModel::InitFromSubMesh(bool softwareSkin)
    {
        ReleaseSourceData();
        ReleaseIndexBuffer();
        ReleaseVertexDeclaration();

        mVertexDecl = Driver->CreateVertexDeclaration();
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
       
        for (auto it = mMesh->mVertexData.begin();
            it != mMesh->mVertexData.end(); ++it)
        {
            DiVertexBuffer* buf = Driver->CreateVertexBuffer();
            uint32 size = it->second.GetSize();
            buf->SetStride(it->second.stride);
            buf->Create(size, RU_WRITE_ONLY,it->first);
            buf->WriteData(0, size, it->second.data);
            mSourceData.push_back(buf);
        }

        mVerticesNum = mMesh->GetVerticeNum();

        // setup software skinning data
        if (mMesh->GetParentMesh()->NeedSoftSkinning())
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

        SetMaterial(mMesh->GetMaterialName());
    }
}