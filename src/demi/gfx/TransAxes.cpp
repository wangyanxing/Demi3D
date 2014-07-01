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
#include "CullNode.h"

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
        mScaleLines->ReleaseSourceData();
        DI_DELETE mScaleLines;
        mScaleLines = nullptr;
        
        for(int i = 0; i < 3; ++i)
        {
            mLines[i]->ReleaseSourceData();
            DI_DELETE mLines[i];
            mLines[i] = nullptr;
            
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
    
    void DiTransAxes::Highlight(PickResult pick)
    {
        DiColor color[3] = {DiColor::Red, DiColor::Green, DiColor::Blue};
        for(int i = 0; i < 3; ++i)
        {
            mColorMats[i]->SetDiffuse(color[i]);
            mShowScalePlane[i] = false;
        }
        
        if(pick == PICK_NONE)
            return;

        if(pick >= PICK_MOVE_X && pick <= PICK_MOVE_Z)
        {
            int id = pick - PICK_MOVE_X;
            mColorMats[id]->SetDiffuse(DiColor::Yellow);
        }
        else if(pick >= PICK_MOVE_XY && pick <= PICK_MOVE_XZ)
        {
            int id = pick - PICK_MOVE_XY;
            mShowScalePlane[id] = true;
        }
        else if(pick >= PICK_SCALE_X && pick <= PICK_SCALE_Z)
        {
            int id = pick - PICK_SCALE_X;
            mColorMats[id]->SetDiffuse(DiColor::Yellow);
        }
        else if(pick >= PICK_SCALE_XY && pick <= PICK_SCALE_XZ)
        {
            int id = pick - PICK_SCALE_XY;
            mShowScalePlane[id] = true;
        }
        else if(pick == PICK_SCALE_XYZ || pick == PICK_MOVE_XYZ)
        {
            for (int i = 0; i < 3; ++i)
            {
                mColorMats[i]->SetDiffuse(DiColor::Yellow);
                mShowScalePlane[i] = true;
            }
        }
    }
    
    DiRenderUnit* DiTransAxes::AddMeshes(const DiVector<GizmoVert>& verts,
                                              DiPrimitiveType primType, DiMaterialPtr mat)
    {
        DiRenderUnit* ru = DI_NEW DiRenderUnit();
        
        ru->mSourceData.push_back(Driver->CreateVertexBuffer());
        ru->mVertexDecl = mVertexDecl;
        
        ru->SetMaterial(mat);
        
        uint32 stride = sizeof(float)*3 + sizeof(uint32);
        uint32 vbsize = verts.size() * stride;
        
        void* vertices = (void*)&verts[0];
        ru->mSourceData[0]->Create(vbsize);
        ru->mSourceData[0]->WriteData(0, vbsize, vertices);
        ru->mSourceData[0]->SetStride(stride);
        
        ru->mVerticesNum = verts.size();
        ru->mPrimitiveType = primType;
        ru->mPrimitiveCount = ru->mVerticesNum;
        
        ru->mEventUpdateTransform = [this](DiMat4* m){
            *m = GetTransform();
        };
        
        return ru;
    }
    
    DiRenderUnit* DiTransAxes::AddMeshes(const DiVector<GizmoVert>& verts,
                                         const DiVector<uint16>& indices,
                                         DiPrimitiveType primType, DiMaterialPtr mat)
    {
        DiRenderUnit* ru = DI_NEW DiRenderUnit();
        
        ru->mSourceData.push_back(Driver->CreateVertexBuffer());
        ru->mVertexDecl = mVertexDecl;
        
        ru->SetMaterial(mat);
        
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
        
        ru->mEventUpdateTransform = [this](DiMat4* m){
            *m = GetTransform();
        };
        
        return ru;
    }

    const DiAABB& DiTransAxes::GetBoundingBox(void) const
    {
        return mBounds;
    }

    void DiTransAxes::AddToBatchGroup(DiRenderBatchGroup* bg)
    {
        if(mShowScale)
            bg->AddRenderUnit(mScaleLines);
        
        for(int i = 0; i < 3; ++i)
        {
            bg->AddRenderUnit(mLines[i]);
            
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
    
    DiTransAxes::PickResult DiTransAxes::Pick(const DiRay& ray)
    {
        if(!mParentNode)
        {
            return PICK_NONE;
        }
        
        DiCullNode* node = dynamic_cast<DiCullNode*>(mParentNode);
        DiMat4 mat = node->GetFullTransform();
        DiSphere center(mat.getTrans(), mPickCenterRadius);
        
        if(mShowArraow)
        {
            if(DiMath::intersects(ray, center).first)
            {
                return PICK_MOVE_XYZ;
            }
                
            for(int i = 0; i < 3; ++i)
            {
                DiVector<DiVec3> verts;
                verts.reserve(mArrawVertices[i].size());
                
                for (auto& v : mArrawVertices[i])
                {
                    verts.push_back(mat.transformAffine(v));
                }
                
                for (size_t j = 0; j < verts.size(); j+=3)
                {
                    auto ret = DiMath::intersects(ray,verts[j],verts[j+1],verts[j+2], true, true);
                    if(ret.first)
                    {
                        return DiTransAxes::PickResult(PICK_MOVE_X + i);
                    }
                }
            }
            
            for(int i = 0; i < 3; ++i)
            {
                DiVector<DiVec3> verts;
                verts.reserve(mScalePlaneVertices[i].size());
                
                for (auto& v : mScalePlaneVertices[i])
                {
                    verts.push_back(mat.transformAffine(v));
                }
                
                for (size_t j = 0; j < verts.size(); j+=3)
                {
                    auto ret = DiMath::intersects(ray,verts[j],verts[j+1],verts[j+2], true, true);
                    if(ret.first)
                    {
                        return DiTransAxes::PickResult(PICK_MOVE_XY + i);
                    }
                }
            }
        }
        
        if(mShowScale)
        {
            if(DiMath::intersects(ray, center).first)
            {
                return PICK_SCALE_XYZ;
            }
            
            for(int i = 0; i < 3; ++i)
            {
                DiVector<DiVec3> verts;
                verts.reserve(mScaleBlockVertices[i].size());
                
                for (auto& v : mScaleBlockVertices[i])
                {
                    verts.push_back(mat.transformAffine(v));
                }
                
                for (size_t j = 0; j < verts.size(); j+=3)
                {
                    auto ret = DiMath::intersects(ray,verts[j],verts[j+1],verts[j+2], true, true);
                    if(ret.first)
                    {
                        return DiTransAxes::PickResult(PICK_SCALE_X + i);
                    }
                }
            }
            
            for(int i = 0; i < 3; ++i)
            {
                DiVector<DiVec3> verts;
                verts.reserve(mScalePlaneVertices[i].size());
                
                for (auto& v : mScalePlaneVertices[i])
                {
                    verts.push_back(mat.transformAffine(v));
                }
                
                for (size_t j = 0; j < verts.size(); j+=3)
                {
                    auto ret = DiMath::intersects(ray,verts[j],verts[j+1],verts[j+2], true, true);
                    if(ret.first)
                    {
                        return DiTransAxes::PickResult(PICK_SCALE_XY + i);
                    }
                }
            }
        }
        
        return PICK_NONE;
    }
    
    void DiTransAxes::SetShowScalePlane(bool v)
    {
        for(int i = 0; i < 3; ++i)
        {
            mShowScalePlane[i] = v;
        }
    }
    
    void DiTransAxes::Create()
    {
        DiColor color[3] = {DiColor::Red, DiColor::Green, DiColor::Blue};
        
        mVertexDecl = Driver->CreateVertexDeclaration();
        mVertexDecl->AddElement(0, VERT_TYPE_FLOAT3, VERT_USAGE_POSITION);
        mVertexDecl->AddElement(0, VERT_TYPE_COLOR,  VERT_USAGE_COLOR);
        mVertexDecl->Create();
        
        // it's a common material
        auto mat = DiMaterial::QuickCreate("_transformGizmo", "basic_v", "basic_p", SHADER_FLAG_USE_COLOR);
        mat->SetDepthCheck(false);
        mat->SetCullMode(CULL_NONE);
        mat->SetBlendMode(BLEND_ALPHA);
        
        mColorMats[0] = DiMaterial::QuickCreate("basic_v", "basic_p", SHADER_FLAG_USE_COLOR);
        mColorMats[1] = DiMaterial::QuickCreate("basic_v", "basic_p", SHADER_FLAG_USE_COLOR);
        mColorMats[2] = DiMaterial::QuickCreate("basic_v", "basic_p", SHADER_FLAG_USE_COLOR);
        
        for(int i = 0; i < 3; ++i)
        {
            mColorMats[i]->SetDepthCheck(false);
            mColorMats[i]->SetCullMode(CULL_NONE);
            mColorMats[i]->SetBlendMode(BLEND_ALPHA);
            mColorMats[i]->SetDiffuse(color[i]);
        }
        
        mLines[0] = AddMeshes(
        {
            GizmoVert(0,0,0, DiColor::White),GizmoVert(3,0,0, DiColor::White)
        }, PT_LINELIST, mColorMats[0]);
        
        mLines[1] = AddMeshes(
        {
            GizmoVert(0,0,0, DiColor::White),GizmoVert(0,3,0, DiColor::White)
        }, PT_LINELIST, mColorMats[1]);
        
        mLines[2] = AddMeshes(
        {
            GizmoVert(0,0,0, DiColor::White),GizmoVert(0,0,3, DiColor::White)
        }, PT_LINELIST, mColorMats[2]);
        
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
        }, PT_LINELIST, mat);
        
        DiColor planeCol(1,1,0,0.3f);
        mScalePlane[0] = AddMeshes(
        {
            GizmoVert(scaleLine1,0,0, planeCol),
            GizmoVert(0,scaleLine1,0, planeCol),
            GizmoVert(0,0,0, planeCol),
        }, PT_TRIANGLELIST, mat);
        
        mScalePlane[1] = AddMeshes(
        {
            GizmoVert(scaleLine1,0,0, planeCol),
            GizmoVert(0,0,scaleLine1, planeCol),
            GizmoVert(0,0,0, planeCol),
        }, PT_TRIANGLELIST, mat);
        
        mScalePlane[2] = AddMeshes(
        {
            GizmoVert(0,scaleLine1,0, planeCol),
            GizmoVert(0,0,scaleLine1, planeCol),
            GizmoVert(0,0,0, planeCol),
        }, PT_TRIANGLELIST, mat);
        
        mScalePlaneVertices[0] = {DiVec3(scaleLine1,0,0),DiVec3(0,scaleLine1,0),DiVec3(0,0,0)};
        mScalePlaneVertices[1] = {DiVec3(scaleLine1,0,0),DiVec3(0,0,scaleLine1),DiVec3(0,0,0)};
        mScalePlaneVertices[2] = {DiVec3(0,scaleLine1,0),DiVec3(0,0,scaleLine1),DiVec3(0,0,0)};
        
        /////////////////////////////////////////////
        
        float axesLineLength = 2.95f;
        float axesOuterLength = 3.55f;
        
        float const radius = 0.15f;
        float const accuracy = 8;
        float MPI = 3.1415926f;
        
        float division = (MPI / 2.0f) / 16.0f;
        float start = division * 3;
        float end = division * 14;

        DiQuat rot[3];
        rot[0] = DiQuat::IDENTITY;
        rot[1].FromAngleAxis(DiRadian(DiDegree(90)), DiVec3::UNIT_Z);
        rot[2].FromAngleAxis(DiRadian(DiDegree(90)), DiVec3::UNIT_Y);
        DiVec3 bias[3] = {DiVec3(1, 0, 0),DiVec3(0, 1, 0),DiVec3(0, 0, 1)};
        
        for(auto i = 0; i < 3; ++i)
        {
            DiVector<GizmoVert> verts;
            DiVector<uint16> ids;
            
            verts.push_back(GizmoVert(bias[i] * axesLineLength, DiColor::White));
            for(float theta = 0; theta < 2 * MPI; theta += MPI / accuracy)
            {
                DiVec3 vet(0, radius * cos(theta), radius * sin(theta));
                vet = rot[i] * vet;
                vet += bias[i] * axesLineLength;
                
                verts.push_back(GizmoVert(vet, DiColor::White));
            }
            verts.push_back(GizmoVert(bias[i] * axesOuterLength, DiColor::White));

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
            
            // save data for picking
            mArrawVertices[i].clear();
            for(auto& id : ids)
            {
                mArrawVertices[i].push_back(DiVec3(verts[id].x,verts[id].y,verts[id].z));
            }
            mArraw[i] = AddMeshes(verts, ids, PT_TRIANGLELIST, mColorMats[i]);
        }
        
        float CUBE_HALF_SIZE = 0.15f;
        for(auto i = 0; i < 3; ++i)
        {
            DiVector<GizmoVert> verts =
            {
                GizmoVert(-CUBE_HALF_SIZE, -CUBE_HALF_SIZE, CUBE_HALF_SIZE, DiColor::White),
                GizmoVert(CUBE_HALF_SIZE, -CUBE_HALF_SIZE, CUBE_HALF_SIZE, DiColor::White),
                GizmoVert(CUBE_HALF_SIZE,  CUBE_HALF_SIZE, CUBE_HALF_SIZE, DiColor::White),
                GizmoVert(-CUBE_HALF_SIZE,  CUBE_HALF_SIZE, CUBE_HALF_SIZE , DiColor::White),
                
                GizmoVert(CUBE_HALF_SIZE, -CUBE_HALF_SIZE, -CUBE_HALF_SIZE, DiColor::White),
                GizmoVert(-CUBE_HALF_SIZE, -CUBE_HALF_SIZE, -CUBE_HALF_SIZE, DiColor::White),
                GizmoVert(-CUBE_HALF_SIZE, CUBE_HALF_SIZE, -CUBE_HALF_SIZE, DiColor::White),
                GizmoVert(CUBE_HALF_SIZE, CUBE_HALF_SIZE, -CUBE_HALF_SIZE, DiColor::White),
                
                GizmoVert(-CUBE_HALF_SIZE, -CUBE_HALF_SIZE, -CUBE_HALF_SIZE, DiColor::White),
                GizmoVert(-CUBE_HALF_SIZE, -CUBE_HALF_SIZE, CUBE_HALF_SIZE, DiColor::White),
                GizmoVert(-CUBE_HALF_SIZE, CUBE_HALF_SIZE, CUBE_HALF_SIZE, DiColor::White),
                GizmoVert(-CUBE_HALF_SIZE, CUBE_HALF_SIZE, -CUBE_HALF_SIZE, DiColor::White),
                
                GizmoVert(CUBE_HALF_SIZE, -CUBE_HALF_SIZE, CUBE_HALF_SIZE, DiColor::White),
                GizmoVert(CUBE_HALF_SIZE, -CUBE_HALF_SIZE, -CUBE_HALF_SIZE, DiColor::White),
                GizmoVert(CUBE_HALF_SIZE, CUBE_HALF_SIZE, -CUBE_HALF_SIZE, DiColor::White),
                GizmoVert(CUBE_HALF_SIZE, CUBE_HALF_SIZE, CUBE_HALF_SIZE, DiColor::White),
                
                GizmoVert(-CUBE_HALF_SIZE, CUBE_HALF_SIZE, CUBE_HALF_SIZE, DiColor::White),
                GizmoVert(CUBE_HALF_SIZE, CUBE_HALF_SIZE, CUBE_HALF_SIZE, DiColor::White),
                GizmoVert(CUBE_HALF_SIZE, CUBE_HALF_SIZE, -CUBE_HALF_SIZE, DiColor::White),
                GizmoVert(-CUBE_HALF_SIZE, CUBE_HALF_SIZE, -CUBE_HALF_SIZE, DiColor::White),

                GizmoVert(-CUBE_HALF_SIZE, -CUBE_HALF_SIZE, -CUBE_HALF_SIZE, DiColor::White),
                GizmoVert(CUBE_HALF_SIZE, -CUBE_HALF_SIZE, -CUBE_HALF_SIZE, DiColor::White),
                GizmoVert(CUBE_HALF_SIZE, -CUBE_HALF_SIZE, CUBE_HALF_SIZE, DiColor::White),
                GizmoVert(-CUBE_HALF_SIZE, -CUBE_HALF_SIZE, CUBE_HALF_SIZE, DiColor::White),
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
            
            // save data for picking
            mScaleBlockVertices[i].clear();
            for(auto& id : ids)
            {
                mScaleBlockVertices[i].push_back(DiVec3(verts[id].x,verts[id].y,verts[id].z));
            }
            mScaleBlock[i] = AddMeshes(verts, ids, PT_TRIANGLELIST, mColorMats[i]);
        }
        
        mBounds.Merge(DiVec3(4,0,0));
        mBounds.Merge(DiVec3(0,4,0));
        mBounds.Merge(DiVec3(0,0,4));
        
        mPickCenterRadius = 0.4f;
    }
}