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
#include "K2Pch.h"
#include "VertexBuffer.h"
#include "VertexDeclaration.h"
#include "K2TerrainDesc.h"
#include "IndexBuffer.h"
#include "RenderBatchGroup.h"
#include "CullNode.h"
#include "ShaderParam.h"
#include "RenderPipeline.h"
#include "GfxDriver.h"

#include "K2TerrainMap.h"
#include "K2TerrainNode.h"
#include "K2TerrainChunk.h"

namespace Demi
{
    static DiIntVec3 PackVector(const DiVec3& vec)
    {
        DiIntVec3 v;
        v.x = DiMath::Clamp(int((vec.x + 1.0f) * 0.5f * 255.0f),0,255);
        v.y = DiMath::Clamp(int((vec.y + 1.0f) * 0.5f * 255.0f),0,255);
        v.z = DiMath::Clamp(int((vec.z + 1.0f) * 0.5f * 255.0f),0,255);
        return v;
    }

    DiTerrainBatch::DiTerrainBatch(DiTerrainChunk* parent)
        :mParent(parent),DiTerrainBatchBase()
    {

    }

    DiTerrainBatch::~DiTerrainBatch()
    {

    }

    void DiTerrainBatch::GetWorldTransform( DiMat4* xform ) const
    {
        *xform = mParent->GetTransform();
    }

    void DiTerrainBatch::UpdateMaterialParams()
    {
        if (mMaterial)
        {
            DiVec4 worldSize(mParent->mParent->GetGridSize(),
                1.0f / mParent->mParent->GetTextureScale(), 0, 0);

            mMaterial->GetShaderParameter()->WriteFloat4("v_WorldSizes",worldSize);
        }
    }

    //////////////////////////////////////////////////////////////////////////

    DiTerrainChunk::DiTerrainChunk( uint16 x, uint16 y , DiTerrain* parent) : 
        mParent(parent),
        mVertexBuffer(NULL),
        mIndexBuffer(NULL),
        mChunkIDX(x),
        mChunkIDY(y),
        mQuadNode(NULL)
    {
        mChunkTransform.setIdentity();
    }

    DiTerrainChunk::~DiTerrainChunk()
    {
        Unload();
    }

    void DiTerrainChunk::BuildBatches()
    {
        {
            Driver->GetPipeline()->ClearGroup();
            for (auto it = mBatches.begin(); it != mBatches.end(); ++it)
            {
                DI_DELETE *it;
            }
            mBatches.clear();
        }                

        DiMap<K2TerrainTexture, DiVector<uint16>> ids;
        uint32 gridsize = CHUNK_GRID_SIZE * CHUNK_GRID_SIZE;

        DiSet<uint32>& ht = mParent->GetHidedTile();

        for (uint32 i = 0; i < gridsize; i++)
        {
            uint32 realGridID = mParent->GetRealGridId(mChunkIDX,mChunkIDY,i);
            if (!ht.contains(realGridID))
            {
                K2TileLayer& l0 = mParent->GetTextureID(0)[realGridID];
                K2TileLayer& l1 = mParent->GetTextureID(1)[realGridID];

                K2TerrainTexture bytes;
                bytes.diffuse0 = l0.diffuseID;
                bytes.normal0 = l0.normalID;
                bytes.diffuse1 = l1.diffuseID;
                bytes.normal1 = l1.normalID;

                ids[bytes].push_back(i);
            }
        }

        int indicesSize = 16 * 6 * gridsize;

        void* bf = mIndexBuffer->Lock(0,indicesSize);
        uint16* data = static_cast<uint16*>(bf);

        uint16 iboffset = 0;
        for (auto it = ids.begin(); it != ids.end(); ++it)
        {
            DiTerrainBatch* batch   = DI_NEW DiTerrainBatch(this);
            batch->mIndexOffset     = iboffset;
            batch->mVertexDecl      = mParent->mVertexDecl;
            batch->mPrimitiveCount  = it->second.size() * 2;
            batch->mPrimitiveType   = PT_TRIANGLELIST;
            batch->mIndexBuffer     = mIndexBuffer;
            batch->mSourceData.push_back(mVertexBuffer);
            batch->mVertexOffset    = 0;
            batch->mVerticesNum     = (CHUNK_GRID_SIZE + 1) * (CHUNK_GRID_SIZE + 1);
            batch->mMaterial        = mParent->GetMaterial(it->first);
            if (!mParent->GetTextureTable().empty())
            {
                batch->mDiffuseTexture[0] = mParent->GetTextureTable()[it->first.diffuse0];
                batch->mDiffuseTexture[1] = mParent->GetTextureTable()[it->first.diffuse1];
                batch->mNormalTexture[0] = mParent->GetTextureTable()[it->first.normal0];
                batch->mNormalTexture[1] = mParent->GetTextureTable()[it->first.normal1];
            }
            mBatches.push_back(batch);

            for (auto iv = it->second.begin(); iv != it->second.end(); ++iv)
            {
                uint16 i = (uint16)((*iv) % CHUNK_GRID_SIZE);
                uint16 j = (uint16)((*iv) / CHUNK_GRID_SIZE);

                uint16 sz = i + j * (CHUNK_GRID_SIZE + 1);
                uint16 nt = i + (j + 1) * (CHUNK_GRID_SIZE + 1);

                bool fst = true;

                if (i % 2 == 0)
                {
                    if (j % 2 != 0)
                        fst = false;
                }
                else
                {
                    if (j % 2 == 0)
                        fst = false;
                }

                if (fst)
                {
                    *data++ = sz;
                    *data++ = sz + 1;
                    *data++ = nt;

                    *data++ = sz + 1;
                    *data++ = nt + 1;
                    *data++ = nt;
                }
                else
                {
                    *data++ = sz;
                    *data++ = nt + 1;
                    *data++ = nt;

                    *data++ = sz;
                    *data++ = sz + 1;
                    *data++ = nt + 1;
                }
            }

            iboffset += (uint16)it->second.size() * 6;
        }

        mIndexBuffer->Unlock();
    }

    void DiTerrainChunk::BuildVertexBuffer()
    {
        DI_ASSERT(!mVertexBuffer);

        mVertexBuffer = Driver->CreateVertexBuffer();

        uint32 vertSize = mParent->mVertexDecl->GetElements().GetStreamElementsSize(0);
        uint32 vertNum = (CHUNK_GRID_SIZE + 1) * (CHUNK_GRID_SIZE + 1);
        uint32 size = vertSize * vertNum;
        mVertexBuffer->Create(size);
        mVertexBuffer->SetStride(vertSize);

        void* buffer = mVertexBuffer->Lock(0,size);
        BYTE* base = (BYTE*)buffer;

        for (uint32 i=0; i<vertNum; i++)
        {
            DiVec3 vec = mParent->GetPosition(mChunkIDX,mChunkIDY,i);
            mQuadNode->GetBounds().Merge(vec);

            float* pos = (float*)base;
            *pos++ = vec.y;

            ARGB* color = (ARGB*)(pos);
            *color++ = mParent->GetColor(mChunkIDX,mChunkIDY,i);

            BYTE tileIDx,tileIDy;
            tileIDx = (BYTE)(i % ((uint32)CHUNK_GRID_SIZE + 1));
            tileIDy = (BYTE)(i / ((uint32)CHUNK_GRID_SIZE + 1));

            DiVec3 normal,tangent;
            mParent->GetNormalTangent(mChunkIDX,mChunkIDY,i,normal,tangent);
            DiIntVec3 n = PackVector(normal);
            DiIntVec3 t = PackVector(tangent);

            BYTE* st = (BYTE*)(color);
            *st++ = (BYTE)n.x;
            *st++ = (BYTE)n.y;
            *st++ = (BYTE)n.z;
            *st++ = tileIDx;

            *st++ = (BYTE)t.x;
            *st++ = (BYTE)t.y;
            *st++ = (BYTE)t.z;
            *st++ = tileIDy;

            base += vertSize;
        }
        
        mVertexBuffer->Unlock();

        ResetBounds();
    }

    void DiTerrainChunk::Load()
    {
        Unload();

        BuildVertexBuffer();

        BuildIndexBuffer();

        BuildBatches();
    }

    void DiTerrainChunk::Unload()
    {
        for (auto it = mBatches.begin(); it != mBatches.end(); ++it)
        {
            DI_DELETE *it;
        }
        mBatches.clear();

        if (mIndexBuffer)
        {
            mIndexBuffer->Release();
            DI_DELETE mIndexBuffer;
            mIndexBuffer = NULL;
        }

        if (mVertexBuffer)
        {
            mVertexBuffer->Release();
            DI_DELETE mVertexBuffer;
            mVertexBuffer = NULL;
        }
    }

    void DiTerrainChunk::BuildIndexBuffer()
    {
        DI_ASSERT(!mIndexBuffer);

        mIndexBuffer = Driver->CreateIndexBuffer();

        int indicesNums = (CHUNK_GRID_SIZE)*(CHUNK_GRID_SIZE)*6;
        int indicesSize = 16 * indicesNums;

        mIndexBuffer->Create(indicesSize);
    }

    void DiTerrainChunk::AddToBatchGroup(DiRenderBatchGroup* bg)
    {
        for (auto it = mBatches.begin(); it != mBatches.end(); ++it)
            bg->AddRenderUnit(*it);
    }

    void DiTerrainChunk::CorrectChunkPosition()
    {
        DiVec2 pos = mParent->GetChunkCenterPos(mChunkIDX,mChunkIDY);
        DiVec3 v3(pos.x,0,pos.y);

        SetPosition(v3);
    }

    DiIntVec2 DiTerrainChunk::TransformRect( const DiFloatRect& rect )
    {
        DiFloatRect realrect = GetRealRect();
        realrect = rect.Intersect(realrect);

        DiVec3 pos;
        mChunkTransform.getTrans(pos);
        realrect.left    -= pos.x;
        realrect.right    -= pos.x;
        realrect.top    -= pos.z;
        realrect.bottom -= pos.z;

        DiIntVec2 vertId;
        long left    = long(realrect.left / mParent->mDesc->mGridSize);
        long right    = long(realrect.right / mParent->mDesc->mGridSize);
        long top    = long(realrect.top / mParent->mDesc->mGridSize);
        long bottom = long(realrect.bottom / mParent->mDesc->mGridSize);
        vertId.x = left + top * CHUNK_GRID_SIZE;
        vertId.y = right + bottom * CHUNK_GRID_SIZE;

        return vertId;
    }

    DiFloatRect DiTerrainChunk::GetRealRect()
    {
        DiFloatRect rect;
        DiVec3 pos;
        mChunkTransform.getTrans(pos);
        float halfGrid = mParent->mDesc->mGridSize / 2;
        rect.left    = pos.x - halfGrid;
        rect.top    = pos.z - halfGrid;
        rect.right    = pos.x + halfGrid;
        rect.bottom    = pos.z + halfGrid;
        return rect;
    }

    void DiTerrainChunk::NotifyVerticesModified(uint32 vertexBegin, uint32 vertexEnd)
    {
        uint32 vertNum = (CHUNK_GRID_SIZE + 1) * (CHUNK_GRID_SIZE + 1);
        uint32 vertSize = mParent->mVertexDecl->GetElements().GetStreamElementsSize(0);
        DI_ASSERT(vertexBegin < vertNum && vertexEnd < vertNum);
        DI_ASSERT(vertexBegin <= vertexEnd);

        uint32 lockSize = (vertexEnd-vertexBegin+1) * vertSize;
        uint32 offset = vertexBegin * vertSize;

        void* buffer = mVertexBuffer->Lock(offset,lockSize);
        BYTE* base = (BYTE*)buffer;

        for (uint32 i = vertexBegin; i<=vertexEnd; i++)
        {
            float* pos = (float*)base;
            *pos++ = mParent->GetHeight(mChunkIDX,mChunkIDY,i);

            ARGB* color = (ARGB*)(pos);
            *color++ = mParent->GetColor(mChunkIDX,mChunkIDY,i);

            BYTE tileIDx,tileIDy;
            tileIDx = (BYTE)(i % ((uint32)CHUNK_GRID_SIZE + 1));
            tileIDy = (BYTE)(i / ((uint32)CHUNK_GRID_SIZE + 1));

            DiVec3 normal,tangent;
            mParent->GetNormalTangent(mChunkIDX,mChunkIDY,i,normal,tangent);
            DiIntVec3 n = PackVector(normal);
            DiIntVec3 t = PackVector(tangent);

            BYTE* st = (BYTE*)(color);
            *st++ = (BYTE)n.x;
            *st++ = (BYTE)n.y;
            *st++ = (BYTE)n.z;
            *st++ = tileIDx;

            *st++ = (BYTE)t.x;
            *st++ = (BYTE)t.y;
            *st++ = (BYTE)t.z;
            *st++ = tileIDy;

            base += vertSize;
        }

        mVertexBuffer->Unlock();

        ResetBounds();
    }

    void DiTerrainChunk::NotifyVerticesModified( const DiRect& vertRect )
    {
         uint32 x = vertRect.left + vertRect.top * (CHUNK_GRID_SIZE + 1);
         uint32 y = vertRect.right + vertRect.bottom * (CHUNK_GRID_SIZE + 1);

        NotifyVerticesModified(x,y);
    }

    void DiTerrainChunk::ResetBounds()
    {
        DI_ASSERT(mQuadNode);
        mQuadNode->ResetBounds();

        uint32 vertNum = (CHUNK_GRID_SIZE + 1) * (CHUNK_GRID_SIZE + 1);
        for (uint32 i=0; i<vertNum; i++)
        {
            DiVec3 vec = mParent->GetPosition(mChunkIDX,mChunkIDY,i);
            mQuadNode->GetBounds().Merge(vec);
        }

        mQuadNode->MergeBoundsToParents();
    }

    void DiTerrainChunk::GetBatches( DiVector<DiRenderUnit*>& visible )
    {
        DiVector<DiTerrainBatch*>::iterator it,itend;
        itend = mBatches.end();
        for (it = mBatches.begin();it!=itend;++it)
        {
            visible.push_back(*it);
        }
    }

    const DiMat4& DiTerrainChunk::GetTransform( void ) const
    {
        return mChunkTransform;
    }

    void DiTerrainChunk::SetPosition( const DiVec3& pos )
    {
        mChunkTransform.setTrans(pos);
    }

    DiRect DiTerrainChunk::GetGridRect()
    {
        DiRect rect;
        rect.left     = mChunkIDX * CHUNK_GRID_SIZE;
        rect.top      = mChunkIDY * CHUNK_GRID_SIZE;
        rect.right    = rect.left + CHUNK_GRID_SIZE - 1;
        rect.bottom = rect.top    + CHUNK_GRID_SIZE - 1;

        return rect;
    }

    int DiTerrainChunk::GetChunkID()
    {
        return mParent->GetDesc()->mSizeX * mChunkIDX + mChunkIDY;
    }
}