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
#include "RenderUnit.h"
#include "math/MathMisc.h"
#include "math/matrix4.h"
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
        for (auto it = mSourceData.begin(); it != mSourceData.end(); ++it)
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