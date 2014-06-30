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
#include "TransAxes.h"
#include "ShaderManager.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "VertexDeclaration.h"

namespace Demi
{
    DiTransAxes::DiTransAxes(void)
        :DiTransformUnit()
    {
        SetBatchGroup(BATCH_HELPER);

        Create();
    }

    DiTransAxes::~DiTransAxes(void)
    {
        for(auto& ru : mMehses)
        {
            ru->ReleaseSourceData();
            DI_DELETE ru;
        }
        
        if (mVertexDecl)
        {
            mVertexDecl->Release();
            DI_DELETE mVertexDecl;
            mVertexDecl = nullptr;
        }
        
        mMehses.clear();
    }
    
    DiRenderUnit* DiTransAxes::AddMeshes(const DiVector<GizmoVert>& verts,
                                              DiPrimitiveType primType)
    {
        DiRenderUnit* ru = DI_NEW DiRenderUnit();
        
        ru->mSourceData.push_back(Driver->CreateVertexBuffer());
        ru->mVertexDecl = mVertexDecl;
        
        auto mat = ru->SetMaterial("_transformGizmo");
        mat->SetCullMode(CULL_NONE);
        
        uint32 stride = sizeof(float)*3 + sizeof(uint32);
        uint32 vbsize = verts.size() * stride;
        
        void* vertices = (void*)&verts[0];
        ru->mSourceData[0]->Create(vbsize);
        ru->mSourceData[0]->WriteData(0, vbsize, vertices);
        ru->mSourceData[0]->SetStride(stride);
        
        ru->mVerticesNum = verts.size();
        ru->mPrimitiveType = primType;
        ru->mPrimitiveCount = ru->mVerticesNum;
        
        mMehses.push_back(ru);
        
        return ru;
    }
    
    DiRenderUnit* DiTransAxes::AddMeshes(const DiVector<GizmoVert>& verts,
                                              const DiVector<uint16>& indices, DiPrimitiveType primType)
    {
        DiRenderUnit* ru = DI_NEW DiRenderUnit();
        
        ru->mSourceData.push_back(Driver->CreateVertexBuffer());
        ru->mVertexDecl = mVertexDecl;
        
        auto mat = ru->SetMaterial("_transformGizmo");
        mat->SetCullMode(CULL_NONE);
        
        uint32 stride = sizeof(float)*3 + sizeof(uint32);
        uint32 vbsize = verts.size() * stride;
        
        void* vertices = (void*)&verts[0];
        ru->mSourceData[0]->Create(vbsize);
        ru->mSourceData[0]->WriteData(0, vbsize, vertices);
        ru->mSourceData[0]->SetStride(stride);
        
        uint32 ibsize = indices.size() * sizeof(uint16);
        ru->mIndexBuffer = Driver->CreateIndexBuffer();
        ru->mIndexBuffer->Create(ibsize);
        ru->mIndexBuffer->WriteData(0, ibsize, &indices[0]);
        
        ru->mVerticesNum = verts.size();
        ru->mPrimitiveType = primType;

        uint32 indexNum = indices.size();
        
        switch (primType)
        {
        case PT_POINTLIST:
            ru->mPrimitiveCount = indexNum;
            break;
        case PT_LINELIST:
            ru->mPrimitiveCount = indexNum / 2;
            break;
        case PT_LINESTRIP:
            ru->mPrimitiveCount = indexNum - 1;
            break;
        case PT_TRIANGLELIST:
            ru->mPrimitiveCount = indexNum / 3;
            break;
        case PT_TRIANGLESTRIP:
        case PT_TRIANGLEFAN:
            ru->mPrimitiveCount = indexNum - 2;
            break;
        default:
            ru->mPrimitiveCount = 0;
        }
        
        mMehses.push_back(ru);

        return ru;
    }

    void DiTransAxes::GetWorldTransform(DiMat4* xform) const
    {
        *xform = GetTransform();
    }
    
    const DiAABB& DiTransAxes::GetBoundingBox(void) const
    {
        //return mBounds;
        return DiAABB::BOX_INFINITE;
    }

    void DiTransAxes::AddToBatchGroup(DiRenderBatchGroup* bg)
    {
        for(auto& ru : mMehses)
        {
            bg->AddRenderUnit(ru);
        }
    }

    DiString& DiTransAxes::GetType()
    {
        static DiString type = "TransformGizmo";
        return type;
    }

    void DiTransAxes::Update(DiCamera* camera)
    {
    }
    
    void DiTransAxes::Create()
    {
        mVertexDecl = Driver->CreateVertexDeclaration();
        mVertexDecl->AddElement(0, VERT_TYPE_FLOAT3, VERT_USAGE_POSITION);
        mVertexDecl->AddElement(0, VERT_TYPE_COLOR,  VERT_USAGE_COLOR);
        mVertexDecl->Create();
        
        DiMaterial::QuickCreate("_transformGizmo", "basic_v", "basic_p", SHADER_FLAG_USE_COLOR);
        
        AddMeshes(
        {
            GizmoVert(0,0,0, DiColor::Red),    GizmoVert(3,0,0, DiColor::Red),
            GizmoVert(0,0,0, DiColor::Green),  GizmoVert(0,3,0, DiColor::Green),
            GizmoVert(0,0,0, DiColor::Blue),   GizmoVert(0,0,3, DiColor::Blue),
        },   PT_LINELIST);
        
        /////////////////////////////////////////////
        
        float axesLineLength = 2.95f;
        float axesOuterLength = 3.55f;
        
        float const radius = 0.15f;
        float const accuracy = 8;
        float MPI = 3.1415926f;
        
        float division = (MPI / 2.0f) / 16.0f;
        float start = division * 3;
        float end = division * 14;

        DiColor color[3] = {DiColor::Red, DiColor::Green, DiColor::Blue};
        DiQuat rot[3];
        rot[0] = DiQuat::IDENTITY;
        rot[1].FromAngleAxis(DiRadian(DiDegree(90)), DiVec3::UNIT_Z);
        rot[2].FromAngleAxis(DiRadian(DiDegree(90)), DiVec3::UNIT_Y);
        DiVec3 bias[3] = {DiVec3(1, 0, 0),DiVec3(0, 1, 0),DiVec3(0, 0, 1)};
        
        for(auto i = 0; i < 3; ++i)
        {
            DiVector<GizmoVert> verts;
            DiVector<uint16> ids;
            
            verts.push_back(GizmoVert(bias[i] * axesLineLength, color[i]));
            for(float theta = 0; theta < 2 * MPI; theta += MPI / accuracy)
            {
                DiVec3 vet(0, radius * cos(theta), radius * sin(theta));
                vet = rot[i] * vet;
                vet += bias[i] * axesLineLength;
                
                verts.push_back(GizmoVert(vet, color[i]));
            }
            verts.push_back(GizmoVert(bias[i] * axesOuterLength, color[i]));

            for(int inside = 1;inside < 16;inside++)
            {
                ids.push_back(0);
                ids.push_back(inside);
                ids.push_back(inside + 1);
            }
            ids.push_back(0);
            ids.push_back(16);
            ids.push_back(1);
            
            for(int outside = 1;outside < 16;outside++)
            {
                ids.push_back(17);
                ids.push_back(outside);
                ids.push_back(outside + 1);
            }
            ids.push_back(17);
            ids.push_back(16);
            ids.push_back(1);
            
            AddMeshes(verts, ids, PT_TRIANGLELIST);
        }
    }
}