
#include "SubModel.h"
#include "SubMesh.h"
#include "Model.h"
#include "VertexBuffer.h"
#include "VertexDeclaration.h"
#include "IndexBuffer.h"
#include "GfxDriver.h"

namespace Demi 
{
    DiSubModel::DiSubModel( DiModel* parent, DiSubMesh* model )
        :mParent(parent),
        mMesh(model)
    {
        InitFromSubMesh();
    }

    DiSubModel::~DiSubModel()
    {
        ReleaseSourceData();
        ReleaseIndexBuffer();
        ReleaseVertexDeclaration();
    }

    void DiSubModel::GetWorldTransform( DiMat4* xform ) const
    {
        *xform = mParent->GetTransform();
    }

    void DiSubModel::InitFromSubMesh()
    {
        ReleaseSourceData();
        ReleaseIndexBuffer();
        ReleaseVertexDeclaration();

        {
            mVertexDecl = Driver->CreateVertexDeclaration();
            mVertexDecl->AddElements(mMesh->mVertexElems);
            mVertexDecl->Create();
        }

        if (mMesh->mIndexData && mMesh->GetIndexNum() > 0)
        {
            mIndexBuffer = Driver->CreateIndexBuffer();

            uint32 size = mMesh->GetIndexSize() * mMesh->GetIndexNum();
            mIndexBuffer->Create(size,
                mMesh->m32BitIndex ? IB_32BITS : IB_16BITS,
                RU_WRITE_ONLY);

            void* data = mIndexBuffer->Lock(0,size);
            memcpy(data,mMesh->mIndexData,size);
            mIndexBuffer->Unlock();

            mPrimitiveCount = mMesh->GetPrimitiveCount();
            mPrimitiveType  = mMesh->GetPrimitiveType();
        }

        {
            for (auto it = mMesh->mVertexData.begin();
                it != mMesh->mVertexData.end(); ++it)
            {
                DiVertexBuffer* buf = Driver->CreateVertexBuffer();
                uint32 size = it->second.GetSize();
                buf->SetStride(it->second.stride);
                buf->Create(size, RU_WRITE_ONLY,it->first);
                void* data = buf->Lock(0,size);
                memcpy(data,it->second.data,size);
                buf->Unlock();
                mSourceData.push_back(buf);
            }

            mVerticesNum = mMesh->GetVerticeNum();
        }

        SetMaterial(mMesh->GetMaterialName());
    }
}