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
#include "AssetManager.h"
#include "MaterialPredefine.h"
#include "ShaderParam.h"
#include "Scene.h"
#include "MaterialSerial.h"
#include "SceneManager.h"
#include "GfxDriver.h"

#include "K2WaterMap.h"
#include "K2TerrainMap.h"
#include "K2WaterChunk.h"

namespace Demi
{
    DiWaterMap::DiWaterMap( DiTerrain* terrainmap )
        :mTerrainMap(terrainmap),
        mCommonDecl(nullptr),
        mIndexBuffer(nullptr)
    {
        mCommonDecl = DiBase::Driver->CreateVertexDeclaration();
        mIndexBuffer = DiBase::Driver->CreateIndexBuffer();
    }

    void DiWaterMap::UpdateWaterTexture()
    {
        if (!mWaterMapTexture)
        {
            mWaterMapTexture = DiAssetManager::GetInstance().CreateOrReplaceAsset<DiTexture>("_global_wt_tex");
            mWaterMapTexture->SetDimensions(WATER_MAP_SIZE,WATER_MAP_SIZE);
            mWaterMapTexture->SetFormat(PF_A8);
            mWaterMapTexture->SetNumLevels(1);
            //m_pkWaterMapTexture->SetUsage(D3DUSAGE_DYNAMIC);
            //m_pkWaterMapTexture->SetPool(D3DPOOL_DEFAULT);
            mWaterMapTexture->SetResourceUsage(RU_STATIC);
            mWaterMapTexture->CreateTexture();
        }

        uint8* map = mTerrainMap->GetDesc()->mWaterMap;

        if (map)
        {
            uint32 pitch  = 0;
            void *buffer = mWaterMapTexture->LockLevel(0, pitch);
            DI_ASSERT(buffer);

            uint8 *levelDst    = (uint8*)buffer;
            const uint32 levelWidth  = mWaterMapTexture->GetWidthInBlocks();
            const uint32 levelHeight = mWaterMapTexture->GetHeightInBlocks();
            const uint32 rowSrcSize  = levelWidth * mWaterMapTexture->GetBlockSize();
            DI_ASSERT(rowSrcSize <= pitch);

            for(uint32 row=0; row<levelHeight; row++)
            {
                memcpy(levelDst, map, rowSrcSize);
                levelDst += pitch;
                map += rowSrcSize;
            }

            mWaterMapTexture->UnlockLevel(0);
        }
    }

    void DiWaterMap::CreateMaterial()
    {
        UpdateWaterTexture();

        mWaterMaterial = DiAssetManager::GetInstance().GetAsset<DiMaterial>("water_well.mtl");
        DiShaderParameter* pm = mWaterMaterial->GetShaderParameter();
        pm->WriteTexture2D("maskTexture","_global_wt_tex");

//         if (mTerrainMap)
//         {
//             DiString waterMaterial = "todo";
//             DiMaterialSerializer mss;
//             mss.ParseMaterial(waterMaterial, mWaterMaterial.get());
//         }
    }

    void DiWaterMap::ClearWaterMap()
    {
        uint8* map = mTerrainMap->GetDesc()->mWaterMap;
        memset(map,0,WATER_MAP_SIZE*WATER_MAP_SIZE*sizeof(BYTE));

        UpdateWaterTexture();
    }

    void DiWaterMap::CreateCommonResource()
    {
        mCommonDecl->Release();
        mCommonDecl->AddElement(0,VERT_TYPE_FLOAT1,    VERT_USAGE_POSITION);
        mCommonDecl->AddElement(0,VERT_TYPE_UBYTE4,    VERT_USAGE_TEXCOORD, 0);
        mCommonDecl->AddElement(0,VERT_TYPE_UBYTE4,    VERT_USAGE_TEXCOORD, 1);
        mCommonDecl->Create();

        unsigned short faces[6] = {0,1,2,2,1,3 };
        uint32 ibsize = 6*sizeof(uint16);
        mIndexBuffer->Create(ibsize);
        mIndexBuffer->WriteData(0, ibsize, faces);
    }

    void DiWaterMap::Load()
    {
        if (!mTerrainMap->GetDesc()->mWaterMap)
        {
            mTerrainMap->GetDesc()->mWaterMap = DI_NEW BYTE[WATER_MAP_SIZE*WATER_MAP_SIZE];
            memset(mTerrainMap->GetDesc()->mWaterMap,0,WATER_MAP_SIZE*WATER_MAP_SIZE*sizeof(BYTE));
        }

        if (!mTerrainMap->GetDesc()->mWaterHeightMap)
        {
            DiIntVec2 sz = mTerrainMap->GetChunkSize();
            mTerrainMap->GetDesc()->mWaterHeightMap = DI_NEW float[sz.x * sz.y];
            memset(mTerrainMap->GetDesc()->mWaterHeightMap,0,sz.x * sz.y*sizeof(float));
        }

        CreateCommonResource();

        CreateMaterial();
    }

    void DiWaterMap::Unload()
    {
        SAFE_DELETE(mCommonDecl);
        SAFE_DELETE(mIndexBuffer);

        for (auto it = mChunks.begin(); it != mChunks.end(); ++it)
        {
            SAFE_DELETE(*it);
        }
        mChunks.clear();
    }

    DiWaterMap::~DiWaterMap()
    {
        Unload();
    }

    void DiWaterMap::AddChunk(uint16 x, uint16 y)
    {
        DiWaterChunk* wc = DI_NEW DiWaterChunk(x,y,this);
        mChunks.push_back(wc);
    }

    DiWaterChunk* DiWaterMap::GetChunk( int id )
    {
        return mChunks[id];
    }

    void DiWaterMap::SetChunkHeight( int id, float height )
    {
        mChunks[id]->SetHeight(height);
    }
}