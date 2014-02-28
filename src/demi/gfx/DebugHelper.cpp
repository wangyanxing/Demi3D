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
#include "DebugHelper.h"
#include "VertexDeclaration.h"
#include "VertexBuffer.h"
#include "ShaderManager.h"

namespace Demi
{
    DiDebugHelper::DiDebugHelper(void)
        :DiTransformUnit(), DiRenderUnit(),
        mVbSize(0),
        mDirty(true)
    {
        mSourceData.push_back(Driver->CreateVertexBuffer());
        
        mVertexDecl = Driver->CreateVertexDeclaration();
        mVertexDecl->AddElement(0, VERT_TYPE_FLOAT3, VERT_USAGE_POSITION);
        mVertexDecl->AddElement(0, VERT_TYPE_COLOR,  VERT_USAGE_COLOR);
        mVertexDecl->Create();
        
        mPrimitiveType = PT_LINELIST;
        
        SetMaterial(DiMaterial::QuickCreate("basic_v", "basic_p", CSF_USE_COLOR));
    }

    DiDebugHelper::~DiDebugHelper(void)
    {
        ReleaseSourceData();
        ReleaseVertexDeclaration();
    }

    void DiDebugHelper::GetWorldTransform(DiMat4* xform) const
    {
        *xform = GetTransform();
    }
    
    void DiDebugHelper::AddLine(const DiVec3& startPos, const DiVec3& endPos, const DiColor& lineColor)
    {
        mBounds.Merge(startPos);
        mBounds.Merge(endPos);
        mLines.push_back(DebugLine(startPos,endPos,lineColor));
        mDirty = true;
    }
    
    void DiDebugHelper::AddFrustum(const DiFrustum* frustum, const DiColor& lineColor)
    {
        const DiVec3 *pts = frustum->GetWorldSpaceCorners();
        
        // near
        AddLine(pts[0],pts[1],lineColor);
        AddLine(pts[1],pts[2],lineColor);
        AddLine(pts[2],pts[3],lineColor);
        AddLine(pts[3],pts[0],lineColor);
        
        // far
        AddLine(pts[5],pts[4],lineColor);
        AddLine(pts[4],pts[7],lineColor);
        AddLine(pts[7],pts[6],lineColor);
        AddLine(pts[6],pts[5],lineColor);
        
		// left
        AddLine(pts[5],pts[6],lineColor);
        AddLine(pts[6],pts[2],lineColor);
        AddLine(pts[2],pts[1],lineColor);
        AddLine(pts[1],pts[5],lineColor);
		
        // right
        AddLine(pts[4],pts[0],lineColor);
        AddLine(pts[0],pts[3],lineColor);
        AddLine(pts[3],pts[7],lineColor);
        AddLine(pts[7],pts[4],lineColor);
        
		// bottom
		AddLine(pts[6],pts[7],lineColor);
        AddLine(pts[7],pts[3],lineColor);
        AddLine(pts[3],pts[2],lineColor);
        AddLine(pts[2],pts[6],lineColor);
        
		// top
        AddLine(pts[4],pts[5],lineColor);
        AddLine(pts[5],pts[1],lineColor);
        AddLine(pts[1],pts[0],lineColor);
        AddLine(pts[0],pts[4],lineColor);
    }
    
    void DiDebugHelper::AddBoundingBox(const DiAABB& bounds, const DiColor& lineColor)
    {
        DiVec3 min = bounds.GetMaximum();
        DiVec3 max = bounds.GetMinimum();
        
        DiVec3 v1(max.x, min.y, min.z);
        DiVec3 v2(max.x, max.y, min.z);
        DiVec3 v3(min.x, max.y, min.z);
        DiVec3 v4(min.x, min.y, max.z);
        DiVec3 v5(max.x, min.y, max.z);
        DiVec3 v6(min.x, max.y, max.z);
        
        AddLine(min, v1, lineColor);
        AddLine(v1, v2, lineColor);
        AddLine(v2, v3, lineColor);
        AddLine(v3, min, lineColor);
        AddLine(v4, v5, lineColor);
        AddLine(v5, max, lineColor);
        AddLine(max, v6, lineColor);
        AddLine(v6, v4, lineColor);
        AddLine(min, v4, lineColor);
        AddLine(v1, v5, lineColor);
        AddLine(v2, max, lineColor);
        AddLine(v3, v6, lineColor);
    }
    
    void DiDebugHelper::Clear()
    {
        mLines.clear();
        mBounds.SetNull();
        mDirty = true;
    }

    const DiAABB& DiDebugHelper::GetBoundingBox(void) const
    {
        //return mBounds;
        return DiAABB::BOX_INFINITE;
    }

    void DiDebugHelper::AddToBatchGroup(DiRenderBatchGroup* bg)
    {
        bg->AddRenderUnit(this);
    }

    DiString& DiDebugHelper::GetType()
    {
        static DiString type = "DebugHelper";
        return type;
    }

    void DiDebugHelper::Update(DiCamera* camera)
    {
        Flush();
    }
    
    void DiDebugHelper::Flush()
    {
        if(!mDirty)
            return;
        
        int stride =sizeof(float) * 3 + sizeof(ARGB);
        int vertNum = mLines.size() * 2;
        
        mPrimitiveCount = mLines.size();
        mVerticesNum = vertNum;
        
        if(mLines.empty())
        {
            mSourceData[0]->Release();
            return;
        }
        
        uint32 vertSize = stride * vertNum;
        if (mVbSize < vertSize)
        {
            mVbSize = vertSize;
            
            mSourceData[0]->Release();
            mSourceData[0]->SetStride(stride);
            mSourceData[0]->Create(mVbSize);
        }
        
        void* data = mSourceData[0]->Lock(0, vertSize);
        ::memcpy(data,&mLines[0],vertSize);
        mSourceData[0]->Unlock();
        
        mDirty = false;
    }
}