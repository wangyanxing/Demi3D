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
        mLines->ReleaseSourceData();
        DI_DELETE mLines;
        mLines = nullptr;
        
        mScaleLines->ReleaseSourceData();
        DI_DELETE mScaleLines;
        mScaleLines = nullptr;
        
        for(int i = 0; i < 3; ++i)
        {
            mArraw[i]->ReleaseSourceData();
            DI_DELETE mArraw[i];
            mArraw[i] = nullptr;
            
            mScaleBlock[i]->ReleaseSourceData();
            DI_DELETE mScaleBlock[i];
            mScaleBlock[i] = nullptr;
        }
        
        if (mVertexDecl)
        {
            mVertexDecl->Release();
            DI_DELETE mVertexDecl;
            mVertexDecl = nullptr;
        }
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
        return ru;
    }

    void DiTransAxes::GetWorldTransform(DiMat4* xform) const
    {
        *xform = GetTransform();
    }
    
    const DiAABB& DiTransAxes::GetBoundingBox(void) const
    {
        return mBounds;
    }

    void DiTransAxes::AddToBatchGroup(DiRenderBatchGroup* bg)
    {
        bg->AddRenderUnit(mLines);
        
        if(mShowScale)
            bg->AddRenderUnit(mScaleLines);
        
        for(int i = 0; i < 3; ++i)
        {
            if(mShowScale)
                bg->AddRenderUnit(mScaleBlock[i]);
            
            if(mShowArraow)
                bg->AddRenderUnit(mArraw[i]);
            
            if(mShowScalePlane[i])
                bg->AddRenderUnit(mScalePlane[i]);
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
        
        auto mat = DiMaterial::QuickCreate("_transformGizmo", "basic_v", "basic_p", SHADER_FLAG_USE_COLOR);
        mat->SetDepthCheck(false);
        mat->SetBlendMode(BLEND_ALPHA);
        
        mLines = AddMeshes(
        {
            GizmoVert(0,0,0, DiColor::Red),    GizmoVert(3,0,0, DiColor::Red),
            GizmoVert(0,0,0, DiColor::Green),  GizmoVert(0,3,0, DiColor::Green),
            GizmoVert(0,0,0, DiColor::Blue),   GizmoVert(0,0,3, DiColor::Blue),
        }, PT_LINELIST);
        
        float scaleLine0 = 1;
        float scaleLine1 = 1.5;
        mScaleLines = AddMeshes(
        {
            GizmoVert(scaleLine0,0,0, DiColor::Red),    GizmoVert(scaleLine0/2,scaleLine0/2,0, DiColor::Red),
            GizmoVert(scaleLine0,0,0, DiColor::Red),    GizmoVert(scaleLine0/2,0,scaleLine0/2, DiColor::Red),
            
            GizmoVert(0,scaleLine0,0, DiColor::Green),  GizmoVert(scaleLine0/2,scaleLine0/2,0, DiColor::Green),
            GizmoVert(0,scaleLine0,0, DiColor::Green),  GizmoVert(0,scaleLine0/2,scaleLine0/2, DiColor::Green),
            
            GizmoVert(0,0,scaleLine0, DiColor::Blue),   GizmoVert(0,scaleLine0/2,scaleLine0/2, DiColor::Blue),
            GizmoVert(0,0,scaleLine0, DiColor::Blue),   GizmoVert(scaleLine0/2,0,scaleLine0/2, DiColor::Blue),
            
            GizmoVert(scaleLine1,0,0, DiColor::Red),    GizmoVert(scaleLine1/2,scaleLine1/2,0, DiColor::Red),
            GizmoVert(scaleLine1,0,0, DiColor::Red),    GizmoVert(scaleLine1/2,0,scaleLine1/2, DiColor::Red),
            
            GizmoVert(0,scaleLine1,0, DiColor::Green),  GizmoVert(scaleLine1/2,scaleLine1/2,0, DiColor::Green),
            GizmoVert(0,scaleLine1,0, DiColor::Green),  GizmoVert(0,scaleLine1/2,scaleLine1/2, DiColor::Green),
            
            GizmoVert(0,0,scaleLine1, DiColor::Blue),   GizmoVert(0,scaleLine1/2,scaleLine1/2, DiColor::Blue),
            GizmoVert(0,0,scaleLine1, DiColor::Blue),   GizmoVert(scaleLine1/2,0,scaleLine1/2, DiColor::Blue),
        }, PT_LINELIST);
        
        DiColor planeCol(1,1,0,0.3f);
        mScalePlane[0] = AddMeshes(
        {
            GizmoVert(scaleLine1,0,0, planeCol),
            GizmoVert(0,scaleLine1,0, planeCol),
            GizmoVert(0,0,0, planeCol),
        }, PT_TRIANGLELIST);
        
        mScalePlane[1] = AddMeshes(
        {
            GizmoVert(scaleLine1,0,0, planeCol),
            GizmoVert(0,0,scaleLine1, planeCol),
            GizmoVert(0,0,0, planeCol),
        }, PT_TRIANGLELIST);
        
        mScalePlane[2] = AddMeshes(
        {
            GizmoVert(0,scaleLine1,0, planeCol),
            GizmoVert(0,0,scaleLine1, planeCol),
            GizmoVert(0,0,0, planeCol),
        }, PT_TRIANGLELIST);
        
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
            
            mArraw[i] = AddMeshes(verts, ids, PT_TRIANGLELIST);
        }
        
        float CUBE_HALF_SIZE = 0.15f;
        for(auto i = 0; i < 3; ++i)
        {
            DiVector<GizmoVert> verts =
            {
                GizmoVert(-CUBE_HALF_SIZE, -CUBE_HALF_SIZE, CUBE_HALF_SIZE, color[i]),
                GizmoVert(CUBE_HALF_SIZE, -CUBE_HALF_SIZE, CUBE_HALF_SIZE, color[i]),
                GizmoVert(CUBE_HALF_SIZE,  CUBE_HALF_SIZE, CUBE_HALF_SIZE, color[i]),
                GizmoVert(-CUBE_HALF_SIZE,  CUBE_HALF_SIZE, CUBE_HALF_SIZE , color[i]),
                
                GizmoVert(CUBE_HALF_SIZE, -CUBE_HALF_SIZE, -CUBE_HALF_SIZE, color[i]),
                GizmoVert(-CUBE_HALF_SIZE, -CUBE_HALF_SIZE, -CUBE_HALF_SIZE, color[i]),
                GizmoVert(-CUBE_HALF_SIZE, CUBE_HALF_SIZE, -CUBE_HALF_SIZE, color[i]),
                GizmoVert(CUBE_HALF_SIZE, CUBE_HALF_SIZE, -CUBE_HALF_SIZE, color[i]),
                
                GizmoVert(-CUBE_HALF_SIZE, -CUBE_HALF_SIZE, -CUBE_HALF_SIZE, color[i]),
                GizmoVert(-CUBE_HALF_SIZE, -CUBE_HALF_SIZE, CUBE_HALF_SIZE, color[i]),
                GizmoVert(-CUBE_HALF_SIZE, CUBE_HALF_SIZE, CUBE_HALF_SIZE, color[i]),
                GizmoVert(-CUBE_HALF_SIZE, CUBE_HALF_SIZE, -CUBE_HALF_SIZE, color[i]),
                
                GizmoVert(CUBE_HALF_SIZE, -CUBE_HALF_SIZE, CUBE_HALF_SIZE, color[i]),
                GizmoVert(CUBE_HALF_SIZE, -CUBE_HALF_SIZE, -CUBE_HALF_SIZE, color[i]),
                GizmoVert(CUBE_HALF_SIZE, CUBE_HALF_SIZE, -CUBE_HALF_SIZE, color[i]),
                GizmoVert(CUBE_HALF_SIZE, CUBE_HALF_SIZE, CUBE_HALF_SIZE, color[i]),
                
                GizmoVert(-CUBE_HALF_SIZE, CUBE_HALF_SIZE, CUBE_HALF_SIZE, color[i]),
                GizmoVert(CUBE_HALF_SIZE, CUBE_HALF_SIZE, CUBE_HALF_SIZE, color[i]),
                GizmoVert(CUBE_HALF_SIZE, CUBE_HALF_SIZE, -CUBE_HALF_SIZE, color[i]),
                GizmoVert(-CUBE_HALF_SIZE, CUBE_HALF_SIZE, -CUBE_HALF_SIZE, color[i]),

                GizmoVert(-CUBE_HALF_SIZE, -CUBE_HALF_SIZE, -CUBE_HALF_SIZE, color[i]),
                GizmoVert(CUBE_HALF_SIZE, -CUBE_HALF_SIZE, -CUBE_HALF_SIZE, color[i]),
                GizmoVert(CUBE_HALF_SIZE, -CUBE_HALF_SIZE, CUBE_HALF_SIZE, color[i]),
                GizmoVert(-CUBE_HALF_SIZE, -CUBE_HALF_SIZE, CUBE_HALF_SIZE, color[i]),
            };
            
            auto aux = bias[i] * axesLineLength;
            
            for(auto& vet : verts)
            {
                vet.x += aux.x;
                vet.y += aux.y;
                vet.z += aux.z;
            }
            
            DiVector<uint16> ids=
            {
                0,1,2,
                0,2,3,
                4,5,6,
                4,6,7,
                8,9,10,
                8,10,11,
                12,13,14,
                12,14,15,
                16,17,18,
                16,18,19,
                20,21,22,
                20,22,23
            };
            
            mScaleBlock[i] = AddMeshes(verts, ids, PT_TRIANGLELIST);
        }
        
        mBounds.Merge(DiVec3(4,0,0));
        mBounds.Merge(DiVec3(0,4,0));
        mBounds.Merge(DiVec3(0,0,4));
    }
}