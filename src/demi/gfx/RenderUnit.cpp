
#include "RenderUnit.h"
#include "MathMisc.h"
#include "matrix4.h"
#include "AssetManager.h"
#include "Material.h"
#include "ShaderParam.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexDeclaration.h"
#include "RenderPipeline.h"
#include "GfxDriver.h"
#include "MaterialPredefine.h"
#include "GfxDriver.h"

namespace Demi 
{

    DiRenderUnit::DiRenderUnit(void)
    {
        mVerticesNum        = 0;
        mPrimitiveType      = PT_POINTLIST;
        mPrimitiveCount     = 0;
        mVertexDecl         = NULL;
        mIndexBuffer        = NULL;
        mIndexOffset        = 0;
        mVertexOffset       = 0;
        mBoneMatrices       = NULL;
        mBoneNum            = 0;
        mIdentityProjection = false;
        mFlags              = 0;
    }

    DiRenderUnit::~DiRenderUnit(void)
    {
        Driver->GetPipeline()->ClearGroup();
    }

    void DiRenderUnit::GetWorldTransform( DiMat4* xform ) const
    {
        *xform = DiMat4::IDENTITY;
    }

    DiMaterialPtr DiRenderUnit::SetMaterial( const DiString& name )
    {
        mMaterial = DiAssetManager::GetInstance().GetAsset<DiMaterial>(name,true);
        if (!mMaterial)
            mMaterial = DiAssetManager::GetInstance().GetAsset<DiMaterial>(DiMaterialDefine::DEFAULT_MATERIAL,true);
        
        return mMaterial;
    }

    void DiRenderUnit::SetMaterial( DiMaterialPtr mat )
    {
        mMaterial = mat;
    }

    void DiRenderUnit::ReleaseIndexBuffer()
    {
        if (mIndexBuffer)
        {
            mIndexBuffer->Release();
            DI_DELETE mIndexBuffer;
            mIndexBuffer = NULL;
        }
    }

    void DiRenderUnit::ReleaseVertexDeclaration()
    {
        if (mVertexDecl)
        {
            mVertexDecl->Release();
            DI_DELETE mVertexDecl;
            mVertexDecl = NULL;
        }
    }

    void DiRenderUnit::ReleaseSourceData()
    {
        DiVector<DiVertexBuffer*>::iterator it;
        DiVector<DiVertexBuffer*>::iterator itEnd = mSourceData.end();
        for (it = mSourceData.begin(); it != itEnd; ++it)
        {
            if (*it)
            {
                (*it)->Release();
                DI_DELETE *it;
            }
        }

        mSourceData.clear();
    }
}