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
#include "DrvD3D9Pch.h"
#include "D3D9VertexDeclaration.h"
#include "D3D9Driver.h"
#include "D3D9StateCache.h"

namespace Demi
{
    DiD3D9VertexDeclaration::DiD3D9VertexDeclaration()
        :mVertexDecl(nullptr)
    {

    }

    DiD3D9VertexDeclaration::~DiD3D9VertexDeclaration()
    {
        Release();
    }

    void DiD3D9VertexDeclaration::Bind()
    {
        if (mVertexDecl)
        {
            HRESULT hr = DiD3D9Driver::StateCache->SetVertexDeclaration(mVertexDecl);
            DX9_CHKERR(hr);
        }
    }

    void DiD3D9VertexDeclaration::Release()
    {
        if (mVertexDecl)
        {
            mVertexDecl->Release();
            mVertexDecl = nullptr;
        }
        mVertexElements.ClearElements();
    }

    void DiD3D9VertexDeclaration::Create()
    {
        DI_ASSERT(!mVertexDecl);
        DiVector<DiVertexElements::Element> eles = mVertexElements.mVertexElements;

        Sort(eles.begin(), eles.end(),
            [](const DiVertexElements::Element& e0, const DiVertexElements::Element& e1) -> bool
        {
            return e0.Stream < e1.Stream;
        });

        DiVector<D3DVERTEXELEMENT9> d3d9Eles;
        for (auto it = eles.begin(); it != eles.end(); ++it)
        {
            D3DVERTEXELEMENT9 e;
            e.Stream = it->Stream;
            e.Offset = it->Offset;
            e.UsageIndex = it->UsageIndex;
            e.Method = D3DDECLMETHOD_DEFAULT;

            switch (it->Type)
            {
            case VERT_TYPE_FLOAT1:
                e.Type = D3DDECLTYPE_FLOAT1;
                break;
            case VERT_TYPE_FLOAT2:
                e.Type = D3DDECLTYPE_FLOAT2;
                break;
            case VERT_TYPE_FLOAT3:
                e.Type = D3DDECLTYPE_FLOAT3;
                break;
            case VERT_TYPE_FLOAT4:
                e.Type = D3DDECLTYPE_FLOAT4;
                break;
            case VERT_TYPE_UBYTE4:
                e.Type = D3DDECLTYPE_UBYTE4;
                break;
            case VERT_TYPE_COLOR:
                e.Type = D3DDECLTYPE_D3DCOLOR;
                break;
            case VERT_TYPE_SHORT2:
                e.Type = D3DDECLTYPE_SHORT2;
                break;
            case VERT_TYPE_SHORT4:
                e.Type = D3DDECLTYPE_SHORT4;
                break;
            default:
                e.Type = 0xFF;
                DI_WARNING("Unsupported vertex decl type");
            };

            switch (it->Usage)
            {
            case VERT_USAGE_POSITION:
                e.Usage = D3DDECLUSAGE_POSITION;
                break;
            case VERT_USAGE_TEXCOORD:
                e.Usage = D3DDECLUSAGE_TEXCOORD;
                break;
            case VERT_USAGE_BLENDWEIGHT:
                e.Usage = D3DDECLUSAGE_BLENDWEIGHT;
                break;
            case VERT_USAGE_BLENDINDICES:
                e.Usage = D3DDECLUSAGE_BLENDINDICES;
                break;
            case VERT_USAGE_NORMAL:
                e.Usage = D3DDECLUSAGE_NORMAL;
                break;
            case VERT_USAGE_TANGENT:
                e.Usage = D3DDECLUSAGE_TANGENT;
                break;
            case VERT_USAGE_BINORMAL:
                e.Usage = D3DDECLUSAGE_BINORMAL;
                break;
            case VERT_USAGE_COLOR:
                e.Usage = D3DDECLUSAGE_COLOR;
                break;
            };

            d3d9Eles.push_back(e);
        }

        D3DVERTEXELEMENT9 element = D3DDECL_END();
        d3d9Eles.push_back(element);
        mVertexDecl = DiD3D9Driver::CreateVertexDeclaration(&d3d9Eles[0]);
    }
}