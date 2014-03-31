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
#include "K2FoliageChunk.h"
#include "VertexBuffer.h"
#include "K2TerrainMap.h"
#include "ShaderParam.h"
#include "K2TerrainChunk.h"
#include "K2FoliageMap.h"
#include "K2FoliageLayer.h"
#include "VertexDeclaration.h"
#include "IndexBuffer.h"
#include "RandomTable.h"
#include "GfxDriver.h"

namespace Demi
{
    DiFoliageLayerBatch::DiFoliageLayerBatch( DiFoliageChunk* parent,  DiFoliageLayer* layer)
        :mParent(parent),
        mLayer(layer)
    {
        mPrimitiveType = PT_TRIANGLELIST;
        Load();
    }

    DiFoliageLayerBatch::~DiFoliageLayerBatch()
    {
        ReleaseVertexDeclaration();
        ReleaseSourceData();
        ReleaseIndexBuffer();
    }

    void DiFoliageLayerBatch::GetWorldTransform( DiMat4* xform ) const
    {
        DiTerrain* tr = mParent->mParent->GetTerrainMap();
        *xform = tr->GetChunk(mParent->mChunkID.x, mParent->mChunkID.y)->GetTransform();
    }

    void DiFoliageLayerBatch::GenerateGrassQuad( const float *grassPositions, unsigned int grassCount )
    {
        ReleaseVertexDeclaration();
        ReleaseSourceData();
        ReleaseIndexBuffer();

        unsigned int quadCount;
        quadCount = grassCount;

        unsigned int maxUInt16 = std::numeric_limits<uint16>::max();
        
        if(grassCount > maxUInt16)
            return;
        if(quadCount > maxUInt16)
            return;

        if (quadCount <= 0)
        {
            mVerticesNum = 0;
            mPrimitiveCount = 0;
            return;
        }

        mVerticesNum    = 4 * quadCount;
        mPrimitiveCount = 2 * quadCount;

        mVertexDecl = Driver->CreateVertexDeclaration();
        mVertexDecl->AddElement(0,VERT_TYPE_FLOAT3,    VERT_USAGE_POSITION);
        mVertexDecl->AddElement(0,VERT_TYPE_FLOAT2,    VERT_USAGE_TEXCOORD);
        mVertexDecl->Create();

        uint32 vertSize = mVertexDecl->GetElements().GetStreamElementsSize(0);
        uint32 size = vertSize * mVerticesNum;
        DiVertexBuffer* vb = Driver->CreateVertexBuffer();
        vb->Create(size);
        vb->SetStride(vertSize);
        mSourceData.push_back(vb);

        DiVec2 chunkPos = mParent->mParent->GetTerrainMap()->GetChunkCenterPos(mParent->mChunkID.x,mParent->mChunkID.y);

        uint8* buffer = DI_NEW uint8[size];
        float* pReal = static_cast<float*>((void*)buffer);
        
        float rndWidth    = mLayer->mDesc->mMaxWidth  - mLayer->mDesc->mMinWidth;
        float rndHeight = mLayer->mDesc->mMaxHeight - mLayer->mDesc->mMinHeight;

        float minY = DiMath::POS_INFINITY, maxY = DiMath::NEG_INFINITY;

        const float *posPtr = grassPositions;
        for (uint16 i = 0; i < grassCount; ++i)
        {
            float x = *posPtr++;
            float z = *posPtr++;

            float rnd = *posPtr++;
            float halfScaleX = (mLayer->mDesc->mMinWidth + rndWidth * rnd) * 0.5f;
            float scaleY = (mLayer->mDesc->mMinHeight + rndHeight * rnd);

            float angle = *posPtr++;
            float xTrans = DiMath::Cos(angle) * halfScaleX;
            float zTrans = DiMath::Sin(angle) * halfScaleX;

            float x1 = x - xTrans, z1 = z - zTrans;
            float x2 = x + xTrans, z2 = z + zTrans;

            float height = 0;
            mParent->mParent->GetTerrainMap()->GetHeight(chunkPos.x+x,chunkPos.y+z,height);
            float y1 = height;
            float y2 = height;


            *pReal++ = float(x1);
            *pReal++ = float(y1 + scaleY);
            *pReal++ = float(z1);   //pos

            *pReal++ = 0.f; *pReal++ = 0.f;              //uv

            *pReal++ = float(x2);
            *pReal++ = float(y2 + scaleY);
            *pReal++ = float(z2);   //pos
            *pReal++ = 1.f; *pReal++ = 0.f;              //uv

            *pReal++ = float(x1);
            *pReal++ = float(y1);
            *pReal++ = float(z1);   //pos
            *pReal++ = 0.f; *pReal++ = 1.f;              //uv

            *pReal++ = float(x2);
            *pReal++ = float(y2);
            *pReal++ = float(z2);   //pos
            *pReal++ = 1.f; *pReal++ = 1.f;              //uv

            if (y1 < minY)
                minY = y1;
            if (y2 < minY) 
                minY = y2;
            if (y1 + scaleY > maxY) 
                maxY = y1 + scaleY;
            if (y2 + scaleY > maxY) 
                maxY = y2 + scaleY;
        }

        vb->WriteData(0, size, buffer);
        DI_DELETE[] buffer;

        uint32 ibsize = 6 * quadCount * sizeof(uint16);
        mIndexBuffer = Driver->CreateIndexBuffer();
        mIndexBuffer->Create(ibsize);
        
        buffer = DI_NEW uint8[ibsize];
        uint16* pI = static_cast<uint16*>((void*)buffer);

        for (uint16 i = 0; i < quadCount; ++i)
        {
            uint16 offset = i * 4;

            *pI++ = 0 + offset;
            *pI++ = 2 + offset;
            *pI++ = 1 + offset;

            *pI++ = 1 + offset;
            *pI++ = 2 + offset;
            *pI++ = 3 + offset;
        }
        mIndexBuffer->WriteData(0, ibsize, buffer);
        DI_DELETE[] buffer;
    }

    void DiFoliageLayerBatch::GenerateGrassCrossQuads( const float *grassPositions, unsigned int grassCount )
    {
        ReleaseVertexDeclaration();
        ReleaseSourceData();
        ReleaseIndexBuffer();

        unsigned int quadCount;
        quadCount = grassCount * 2;

        unsigned int maxUInt16 = std::numeric_limits<uint16>::max();
        if(grassCount > maxUInt16)
        {
            return ;
        }
        if(quadCount > maxUInt16)
        {
            return ;
        }
        if (quadCount <= 0)
        {
            mVerticesNum = 0;
            mPrimitiveCount = 0;
            return;
        }

        mVerticesNum = 4*quadCount;
        mPrimitiveCount = 2*quadCount;

        mVertexDecl = Driver->CreateVertexDeclaration();
        mVertexDecl->AddElement(0, VERT_TYPE_FLOAT3, VERT_USAGE_POSITION);
        mVertexDecl->AddElement(0, VERT_TYPE_FLOAT2, VERT_USAGE_TEXCOORD);
        mVertexDecl->Create();

        float rndWidth = mLayer->mDesc->mMaxWidth - mLayer->mDesc->mMinWidth;
        float rndHeight = mLayer->mDesc->mMaxHeight - mLayer->mDesc->mMinHeight;
        float minY = DiMath::POS_INFINITY, maxY = DiMath::NEG_INFINITY;

        uint32 vertSize = mVertexDecl->GetElements().GetStreamElementsSize(0);
        uint32 size = vertSize * mVerticesNum;
        DiVertexBuffer* vb = Driver->CreateVertexBuffer();
        vb->Create(size);
        vb->SetStride(vertSize);
        mSourceData.push_back(vb);

        DiVec2 chunkPos = mParent->mParent->GetTerrainMap()->GetChunkCenterPos(mParent->mChunkID.x,mParent->mChunkID.y);

        uint8* buffer = DI_NEW uint8[size];
        float* pReal = static_cast<float*>((void*)buffer);

        const float *posPtr = grassPositions;
        for (uint16 i = 0; i < grassCount; ++i)
        {
            float x = *posPtr++;
            float z = *posPtr++;

            float rnd = *posPtr++;
            float halfScaleX = (mLayer->mDesc->mMinWidth + rndWidth * rnd) * 0.5f;
            float scaleY = (mLayer->mDesc->mMinHeight + rndHeight * rnd);

            float angle = *posPtr++;
            float xTrans = DiMath::Cos(angle) * halfScaleX;
            float zTrans = DiMath::Sin(angle) * halfScaleX;

            float x1 = x - xTrans, z1 = z - zTrans;
            float x2 = x + xTrans, z2 = z + zTrans;

            float height = 0;
            mParent->mParent->GetTerrainMap()->GetHeight(chunkPos.x+x,chunkPos.y+z,height);
            float y1 = height;
            float y2 = height;
        
            *pReal++ = float(x1);
            *pReal++ = float(y1 + scaleY);
            *pReal++ = float(z1);   //pos
            *pReal++ = 0.f; *pReal++ = 0.f;              //uv

            *pReal++ = float(x2);
            *pReal++ = float(y2 + scaleY);
            *pReal++ = float(z2);   //pos
            *pReal++ = 1.f; *pReal++ = 0.f;              //uv

            *pReal++ = float(x1);
            *pReal++ = float(y1);
            *pReal++ = float(z1);   //pos
            *pReal++ = 0.f; *pReal++ = 1.f;              //uv

            *pReal++ = float(x2);
            *pReal++ = float(y2);
            *pReal++ = float(z2);   //pos
            *pReal++ = 1.f; *pReal++ = 1.f;              //uv

            if (y1 < minY)
            {
                minY = y1;
            }
            if (y2 < minY) 
            {
                minY = y2;
            }
            if (y1 + scaleY > maxY)
            {
                maxY = y1 + scaleY;
            }
            if (y2 + scaleY > maxY) 
            {
                maxY = y2 + scaleY;
            }

            float x3 = x + zTrans, z3 = z - xTrans;
            float x4 = x - zTrans, z4 = z + xTrans;

            float y3 = 0.f, y4 = 0.f;

            *pReal++ = float(x3);
            *pReal++ = float(y3 + scaleY);
            *pReal++ = float(z3);   //pos
            *pReal++ = 0.f; *pReal++ = 0.f;              //uv

            *pReal++ = float(x4);
            *pReal++ = float(y4 + scaleY);
            *pReal++ = float(z4);   //pos
            *pReal++ = 1.f; *pReal++ = 0.f;              //uv

            *pReal++ = float(x3);
            *pReal++ = float(y3); 
            *pReal++ = float(z3);   //pos
            *pReal++ = 0.f; *pReal++ = 1.f;              //uv

            *pReal++ = float(x4);
            *pReal++ = float(y4);
            *pReal++ = float(z4);   //pos
            *pReal++ = 1.f; *pReal++ = 1.f;              //uv

            if (y3 < minY) 
            {
                minY = y1;
            }
            if (y4 < minY) 
            {
                minY = y2;
            }
            if (y3 + scaleY > maxY) 
            {
                maxY = y3 + scaleY;
            }
            if (y4 + scaleY > maxY)
            {
                maxY = y4 + scaleY;
            }
        }

        vb->WriteData(0,size,buffer);
        DI_DELETE[] buffer;

        uint32 ibsize = 6 * quadCount * sizeof(uint16);
        mIndexBuffer = Driver->CreateIndexBuffer();
        mIndexBuffer->Create(ibsize);
        buffer = DI_NEW uint8[ibsize];
        uint16* pI = static_cast<uint16*>((void*)buffer);

        for (uint16 i = 0; i < quadCount; ++i)
        {
            uint16 offset = i * 4;

            *pI++ = 0 + offset;
            *pI++ = 2 + offset;
            *pI++ = 1 + offset;

            *pI++ = 1 + offset;
            *pI++ = 2 + offset;
            *pI++ = 3 + offset;
        }
        mIndexBuffer->WriteData(0, ibsize, buffer);
        DI_DELETE[] buffer;
    }

    void DiFoliageLayerBatch::GenerateGrassSprite( const float *grassPositions, unsigned int grassCount )
    {
        ReleaseVertexDeclaration();
        ReleaseSourceData();
        ReleaseIndexBuffer();

        // TODO

    }

    uint32 DiFoliageLayerBatch::PopulateGrassList( float *posBuff, unsigned int grassCount )
    {
        float *posPtr = posBuff;

        mParent->GetRandomTable()->ResetRandomIndex();

        DiVec2 size = mParent->mParent->GetTerrainMap()->GetWorldSize();
        float chunksize = mParent->mParent->GetTerrainMap()->GetGridSize() * CHUNK_GRID_SIZE;
        DiFloatRect mapBounds(0,0,size.x,size.y);
        float xTerSpaceBase = mParent->mChunkID.x * chunksize;
        float zTerSpaceBase = mParent->mChunkID.y * chunksize;

        for (unsigned int i = 0; i < grassCount; ++i)
        {
            float x = mParent->GetRandomTable()->GetRangeRandom(0,
                CHUNK_GRID_SIZE * mParent->mParent->GetTerrainMap()->GetGridSize());
            float z = mParent->GetRandomTable()->GetRangeRandom(0,
                CHUNK_GRID_SIZE * mParent->mParent->GetTerrainMap()->GetGridSize());

            if (mParent->GetRandomTable()->GetUnitRandom() < 
                mLayer->GetDensityMap()->GetDensityAt(xTerSpaceBase+x, zTerSpaceBase+z, mapBounds))
            {
                *posPtr++ = x;
                *posPtr++ = z;

                *posPtr++ = mParent->GetRandomTable()->GetUnitRandom();
                *posPtr++ = mParent->GetRandomTable()->GetRangeRandom(0, (float)DiMath::TWO_PI);
            }
        }

        grassCount = (posPtr - posBuff) / 4;
        return grassCount;
    }

    void DiFoliageLayerBatch::Load()
    {
        float bound = CHUNK_GRID_SIZE * mParent->mParent->GetTerrainMap()->GetGridSize();
        float volume = bound * bound;

        uint32 grassCount = (uint32)(mLayer->mDesc->mDensity * volume);

        float *position = DI_NEW float[grassCount*4];
        grassCount = PopulateGrassList(position,grassCount);

        if (mLayer->mDesc->mRenderTechnique == GRASSTECH_QUAD)
        {
            GenerateGrassQuad(position, grassCount);
        }
        else if (mLayer->mDesc->mRenderTechnique == GRASSTECH_CROSSQUADS)
        {
            GenerateGrassCrossQuads(position, grassCount);
        }

        SetMaterial(mLayer->GetMaterial());

        DI_DELETE[] position;
    }

    //////////////////////////////////////////////////////////////////////////

    void DiFoliageChunk::BuildBatches()
    {
        CleanBatches();

        int layernums = mParent->GetLayerNums();
        for (int i=0; i < layernums; i++)
        {
            DiFoliageLayer* layer = mParent->GetLayer(i);
            DiFoliageLayerBatch* lb = DI_NEW DiFoliageLayerBatch(this,layer);
            mLayerBatches[(unsigned long)layer] = lb;
        }
    }

    void DiFoliageChunk::CleanBatches()
    {
        LayerBatches::iterator it;
        LayerBatches::iterator itEnd = mLayerBatches.end();
        for ( it = mLayerBatches.begin(); it != itEnd; ++it)
        {
            SAFE_DELETE(it->second);
        }
        mLayerBatches.clear();
    }

    void DiFoliageChunk::GetBatches( DiVector<DiRenderUnit*>& visible )
    {
        LayerBatches::iterator it;
        LayerBatches::iterator itEnd = mLayerBatches.end();
        for ( it = mLayerBatches.begin(); it != itEnd; ++it)
        {
            visible.push_back(it->second);
        }
    }

    DiRandomTable* DiFoliageChunk::GetRandomTable()
    {
        return mParent->mTable;
    }

    DiFoliageChunk::DiFoliageChunk( uint16 x, uint16 y, DiFoliageMap* parent )
        :mParent(parent),
        mChunkID(x,y)
    {
        BuildBatches();
    }

    DiFoliageChunk::~DiFoliageChunk()
    {
        CleanBatches();
    }

    void DiFoliageChunk::RebuildBatch( DiFoliageLayer* layer )
    {
        if( layer )
        {
            DiFoliageLayerBatch* batch = mLayerBatches.find_def((unsigned long)layer,NULL);
            if (batch)
                batch->Load();
            else
            {
                DiFoliageLayerBatch* lb = DI_NEW DiFoliageLayerBatch(this,layer);
                mLayerBatches[(unsigned long)layer] = lb;
            }
        }
        else
        {
            LayerBatches::iterator it;
            LayerBatches::iterator itEnd = mLayerBatches.end();
            for (it = mLayerBatches.begin(); it != itEnd; ++it)
            {
                if (it->second)
                    it->second->Load();
            }
        }
    }

    void DiFoliageChunk::DeleteBatch( DiFoliageLayer* l )
    {
        LayerBatches::iterator it = mLayerBatches.find((unsigned long)l);
        if (it != mLayerBatches.end())
        {
            SAFE_DELETE(it->second);
            mLayerBatches.erase(it);
        }
    }
}