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
#include "GfxDriver.h"
#include "ShaderParam.h"

#include "K2TerrainMap.h"
#include "K2WaterChunk.h"
#include "K2WaterMap.h"
#include "K2TerrainChunk.h"

namespace Demi
{
    DiWaterChunk::DiWaterChunk( uint16 x, uint16 y, DiWaterMap* parent )
        :mParent(parent),
        mChunkID(x,y),
        mVertexBuffer(NULL),
        mHeight(10)
    {
        mPrimitiveType = PT_TRIANGLELIST;
        mPrimitiveCount = 2;
        mVerticesNum = 4;
        mMaterial = parent->mWaterMaterial;

        BuildBuffers();
    }

    DiWaterChunk::~DiWaterChunk()
    {
        ReleaseSourceData();
    }

    void DiWaterChunk::BuildBuffers()
    {
        mVertexDecl = mParent->mCommonDecl;
        mIndexBuffer = mParent->mIndexBuffer;

        uint32 vertSize = mVertexDecl->GetElements().GetStreamElementsSize(0);

        uint32 vertNum = 4;
        uint32 size = vertSize * vertNum;

        if (!mVertexBuffer)
        {
            mVertexBuffer = DiBase::Driver->CreateVertexBuffer();
            mVertexBuffer->Create(size);
            mVertexBuffer->SetStride(vertSize);
            mSourceData.push_back(mVertexBuffer);
        }

        void* buffer = mVertexBuffer->Lock(0,size);

        DiIntVec3 n(128,255,128);
        DiIntVec3 t(255,128,128);

        DiTerrainDescPtr tr = mParent->mTerrainMap->GetDesc();
        uint32 id = mChunkID.x * tr->mSizeX + mChunkID.y;
        float height = tr->mWaterHeightMap[id];
        mHeight = height;

        BYTE* base = (BYTE*)buffer;

        float* pos = (float*)base;
        *pos++ = height;
        BYTE* st = (BYTE*)(pos);
        *st++ = (BYTE)n.x;
        *st++ = (BYTE)n.y;
        *st++ = (BYTE)n.z;
        *st++ = (BYTE)0;
        *st++ = (BYTE)t.x;
        *st++ = (BYTE)t.y;
        *st++ = (BYTE)t.z;
        *st++ = (BYTE)0;

        pos = (float*)st;
        *pos++ = height;
        st = (BYTE*)(pos);
        *st++ = (BYTE)n.x;
        *st++ = (BYTE)n.y;
        *st++ = (BYTE)n.z;
        *st++ = (BYTE)0;
        *st++ = (BYTE)t.x;
        *st++ = (BYTE)t.y;
        *st++ = (BYTE)t.z;
        *st++ = (BYTE)CHUNK_GRID_SIZE;

        pos = (float*)st;
        *pos++ = height;
        st = (BYTE*)(pos);
        *st++ = (BYTE)n.x;
        *st++ = (BYTE)n.y;
        *st++ = (BYTE)n.z;
        *st++ = (BYTE)CHUNK_GRID_SIZE;
        *st++ = (BYTE)t.x;
        *st++ = (BYTE)t.y;
        *st++ = (BYTE)t.z;
        *st++ = (BYTE)0;

        pos = (float*)st;
        *pos++ = height;
        st = (BYTE*)(pos);
        *st++ = (BYTE)n.x;
        *st++ = (BYTE)n.y;
        *st++ = (BYTE)n.z;
        *st++ = (BYTE)CHUNK_GRID_SIZE;
        *st++ = (BYTE)t.x;
        *st++ = (BYTE)t.y;
        *st++ = (BYTE)t.z;
        *st++ = (BYTE)CHUNK_GRID_SIZE;
        
        mVertexBuffer->Unlock();
    }

    void DiWaterChunk::UpdateMaterial()
    {
        DiTerrain* tr = mParent->mTerrainMap;
        DiVec4 worldSize(tr->GetGridSize(),tr->GetTextureScale(),tr->GetWorldSize().x,tr->GetWorldSize().y);

        mMaterial->GetShaderParameter()->WriteFloat4("v_WorldSizes",worldSize);
    }

    void DiWaterChunk::GetWorldTransform( DiMat4* xform ) const
    {
        DiTerrain* tr = mParent->mTerrainMap;
        *xform = tr->GetChunk(mChunkID.x,mChunkID.y)->GetTransform();
    }

    void DiWaterChunk::SetHeight( float val )
    {
        if (!DiMath::RealEqual(mHeight,val))
        {
            mHeight = val;

            DiTerrainDescPtr tr = mParent->mTerrainMap->GetDesc();
            uint32 id = mChunkID.x * tr->mSizeX + mChunkID.y;
            tr->mWaterHeightMap[id] = val;

            BuildBuffers();
        }
    }
}