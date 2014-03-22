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
#include "RenderUnit.h"
#include "RenderBatchGroup.h"
#include "Camera.h"
#include "Scene.h"
#include "AssetManager.h"
#include "VertexDeclaration.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "SceneManager.h"
#include "CullNode.h"
#include "MaterialPredefine.h"
#include "Texture.h"
#include "ShaderParam.h"
#include "GfxDriver.h"

#include "K2TerrainNode.h"
#include "K2WaterMap.h"
#include "K2WaterChunk.h"
#include "K2FoliageChunk.h"
#include "K2FoliageMap.h"
#include "K2FoliageLayer.h"
#include "K2TerrainMap.h"
#include "K2TerrainChunk.h"

#if DEMI_COMPILER == DEMI_COMPILER_MSVC
#   pragma warning(disable:4505)
#endif

namespace Demi
{
    DiTerrainMap::DiTerrainMap()
       :mVertexDecl(nullptr),
        mVertexBuffer(nullptr),
        mDesc(nullptr),
        mMaxHeight(-10000000),
        mMinHeight(10000000),
        mWaterMap(nullptr),
        mFoliageMap(nullptr),
        mRoot(nullptr)
    {
        SetShadowCastEnable(false);
        CreateVertexDecl();
    }

    DiTerrainMap::~DiTerrainMap()
    {
        if (mVertexDecl)
        {
            mVertexDecl->Release();
            DI_DELETE mVertexDecl;
            mVertexDecl = nullptr;
        }
    }

    bool DiTerrainMap::Load(DiTerrainDescPtr desc)
    {
        if (!desc->CheckValid())
        {
            DI_WARNING("Invalid terrain description");
            return false;
        }

        mDesc = desc;

        uint32 vertSize = mDesc->GetVertNum(); 
        uint32 gridSize = mDesc->GetGridNum();

        if (mDesc->mHeightData)
        {
            for (size_t i = 0; i<vertSize; i++)
            {
                if (mDesc->mHeightData[i] > mMaxHeight)
                    mMaxHeight = mDesc->mHeightData[i];
                if (mDesc->mHeightData[i] < mMinHeight)
                    mMinHeight = mDesc->mHeightData[i];
            }
        }
        else
        {
            mDesc->mHeightData = DI_NEW float[vertSize];
            memset(mDesc->mHeightData, 0, vertSize*sizeof(float));
            mMaxHeight = mMinHeight = 0;
        }

        if (!mDesc->mColorData)
        {
            mDesc->mColorData = DI_NEW ARGB[vertSize];
            for (uint32 s = 0; s < vertSize; s++)
            {
                DiColor c(1, 1, 1, 0);
                mDesc->mColorData[s] = c.GetAsARGB();
            }
        }

        for (uint32 i = 0; i < TERRAIN_LAYER_NUM; i++)
        {
            if (!mDesc->mLayers[i].textureId)
            {
                mDesc->mLayers[i].textureId = DI_NEW uint8[gridSize];
                memset(mDesc->mLayers[i].textureId, 0, gridSize*sizeof(uint8));
            }
        }

        if (mDesc->mWaterMap)
            LoadWaterMap();

        LoadFoliageMap();

        if (!mDesc->mCliffData)
        {
            uint32 cliffVertNum = mDesc->GetCliffVertNum();
            mDesc->mCliffData = DI_NEW char[cliffVertNum];
            memset(mDesc->mCliffData, 0/*CLIFF_A*/, cliffVertNum*sizeof(char));
        }

        for (uint16 x = 0; x < mDesc->mSizeX; ++x)
        {
            for (uint16 y = 0; y < mDesc->mSizeY; ++y)
            {
                DiTerrainChunk* chunk = DI_NEW DiTerrainChunk(x, y, this);
                chunk->CorrectChunkPosition();
                mChunks.push_back(chunk);
            }
        }

        SAFE_DELETE(mRoot);

        uint16 quadsize = (uint16)DiMath::FirstPO2From(DiMath::Max(mDesc->mSizeX, mDesc->mSizeY));
        mRoot = DI_NEW DiQuadNode(this, NULL, quadsize, 0, 0);

        for (auto it = mChunks.begin(); it != mChunks.end(); ++it)
            (*it)->Load();

        return true;
    }

    void DiTerrainMap::Unload()
    {
        for (auto it = mChunks.begin(); it != mChunks.end(); ++it)
        {
            DiTerrainChunk* tc = *it;
            DI_DELETE tc;
        }
        mChunks.clear();
        
        SAFE_DELETE(mWaterMap);            
        SAFE_DELETE(mFoliageMap);
        
        mDesc = nullptr;

        SAFE_DELETE(mRoot);
    }

    void DiTerrainMap::CreateVertexDecl()
    {
        DI_ASSERT(!mVertexDecl);

        mVertexDecl = Driver->CreateVertexDeclaration();
        mVertexDecl->AddElement(0, VERT_TYPE_FLOAT1, VERT_USAGE_POSITION);
        mVertexDecl->AddElement(0, VERT_TYPE_COLOR,  VERT_USAGE_COLOR);
        mVertexDecl->AddElement(0, VERT_TYPE_UBYTE4, VERT_USAGE_TEXCOORD, 0);
        mVertexDecl->AddElement(0, VERT_TYPE_UBYTE4, VERT_USAGE_TEXCOORD, 1);
        mVertexDecl->Create();
    }
    
    ARGB DiTerrainMap::GetColor( uint32 vertid )
    {
        uint32 size = (CHUNK_GRID_SIZE*mDesc->mSizeX + 1) * 
            (CHUNK_GRID_SIZE*mDesc->mSizeY + 1);

        DI_ASSERT(vertid < size);

        return mDesc->mColorData[vertid];
    }

    ARGB DiTerrainMap::GetColor( uint16 trunkIDx, uint16 trunkIDy, uint32 vertid )
    {
        uint32 realid = GetRealVertId(trunkIDx,trunkIDy,vertid);
        return GetColor(realid);
    }

    float DiTerrainMap::GetHeight( uint16 trunkIDx, uint16 trunkIDy, uint32 vertid )
    {
        uint32 realid = GetRealVertId(trunkIDx,trunkIDy,vertid);

        uint32 size = (CHUNK_GRID_SIZE*mDesc->mSizeX + 1) * 
            (CHUNK_GRID_SIZE*mDesc->mSizeY + 1);
        DI_ASSERT(vertid < size);

        return mDesc->mHeightData[realid];
    }

    bool DiTerrainMap::GetHeight( float worldX, float worldZ, float& outHeight )
    {
        DiVec3 pos(worldX,0,worldZ);
        if(!CoverTerrain(pos))
        {
            return false;
        }

        DiIntVec2 vert = GetVertexPos(pos);
        
        DiRay ray;
        ray.setOrigin(DiVec3(worldX,1000,worldZ));
        ray.setDirection(DiVec3::NEGATIVE_UNIT_Y);
        float outpos;
        if(QuadRayIntersects(vert.x,vert.y,ray,outpos))
        {
            outHeight = ray.getPoint(outpos).y;
            return true;
        }
        else
        {
            return false;
        }
    }

    uint32 DiTerrainMap::GetRealVertId( uint16 trunkIDx, uint16 trunkIDy, uint32 vertid )
    {
        uint32 tileIDx,tileIDy;
        tileIDx = vertid % (CHUNK_GRID_SIZE + 1);
        tileIDy = vertid / (CHUNK_GRID_SIZE + 1);

        uint32 realx = trunkIDx * (CHUNK_GRID_SIZE ) + tileIDx;
        uint32 realy = trunkIDy * (CHUNK_GRID_SIZE ) + tileIDy;

        uint32 realid = realy * (CHUNK_GRID_SIZE * mDesc->mSizeX + 1) + realx;
        return realid;
    }

    uint32 DiTerrainMap::GetRealGridId( uint16 trunkIDx, uint16 trunkIDy, uint32 gridID )
    {
        uint32 tileIDx,tileIDy;
        tileIDx = gridID % (CHUNK_GRID_SIZE);
        tileIDy = gridID / (CHUNK_GRID_SIZE);

        uint32 realx = trunkIDx * CHUNK_GRID_SIZE + tileIDx;
        uint32 realy = trunkIDy * CHUNK_GRID_SIZE + tileIDy;

        uint32 realid = realy * (mDesc->mSizeX * CHUNK_GRID_SIZE) + realx;
        return realid;
    }

    DiMaterialPtr DiTerrainMap::GetMaterial(const DiPair<BYTE,BYTE>& textureid)
    {
        DiTerrainDesc::TextureTable& texTable = GetTextureTable();
        if(texTable.empty())
            return DiAssetManager::GetInstance().GetAsset<DiMaterial>(DiMaterialDefine::DEFAULT_MATERIAL);

        if( mMaterialTable.contains(textureid) )
            return mMaterialTable[textureid];
        else
        {
            DiString texlayer0,texlayer1;
            if (texTable.contains(textureid.first))
                texlayer0 = texTable[textureid.first];
            else
            {
                DI_WARNING("Cannot find the texture ID0: %d",textureid.first);
            }

            if (texTable.contains(textureid.second))
                texlayer1 = texTable[textureid.second];
            else
            {
                DI_WARNING("Cannot find the texture ID1: %d",textureid.second);
            }

            DiMaterialPtr mat = GenerateMaterial(texlayer0,texlayer1);
            mMaterialTable[textureid] = mat;
            return mat;
        }
    }

    float DiTerrainMap::GetHeightAtPoint( uint32 x, uint32 y )
    {
        uint32 vertx,verty;
        GetVerticesNum(vertx,verty);

        x = DiMath::Min(x, vertx - 1);
        x = DiMath::Max(x, 0);
        y = DiMath::Min(y, verty - 1);
        y = DiMath::Max(y, 0);

        return *GetHeightData(x, y);
    }

    float* DiTerrainMap::GetHeightData()
    {
        return mDesc->mHeightData;
    }

    float* DiTerrainMap::GetHeightData( uint32 x, uint32 y )
    {
        uint32 vertx,verty;
        GetVerticesNum(vertx,verty);

        DI_ASSERT (x >= 0 && x < vertx && y >= 0 && y < verty);
        return &mDesc->mHeightData[y * vertx + x];
    }

    void DiTerrainMap::GetVerticesNum( uint32& outx, uint32& outy )
    {
        outx = (CHUNK_GRID_SIZE * mDesc->mSizeX + 1);
        outy = (CHUNK_GRID_SIZE * mDesc->mSizeY + 1);
    }

    Demi::DiIntVec2 DiTerrainMap::GetVerticesNum()
    {
        DiIntVec2 res;
        res.x = (int)(CHUNK_GRID_SIZE * mDesc->mSizeX + 1);
        res.y = (int)(CHUNK_GRID_SIZE * mDesc->mSizeY + 1);
        return res;
    }

    DiIntVec2 DiTerrainMap::GetVertexPos( const DiVec3& WSpos )
    {
        uint32 vertx,verty;
        GetVerticesNum(vertx,verty);

        float wholeSizeX = mDesc->mGridSize * mDesc->mSizeX * CHUNK_GRID_SIZE;
        float wholeSizeY = mDesc->mGridSize * mDesc->mSizeY * CHUNK_GRID_SIZE;

        float x = (WSpos.x + wholeSizeX/2) / (mDesc->mGridSize);
        float y = (WSpos.z + wholeSizeY/2) / (mDesc->mGridSize);

        DiIntVec2 tpos;
        tpos.x = int(x);
        tpos.y = int(y);

        return tpos;
    }

    DiTerrainChunk* DiTerrainMap::GetChunk( uint32 idx, uint32 idy )
    {
        uint32 id = idx * mDesc->mSizeX + idy;
        if(id < mChunks.size())
            return mChunks[id];
        else
            return nullptr;
    }

    DiTerrainChunk* DiTerrainMap::GetChunk( uint32 id )
    {
        return mChunks[id];
    }

    ARGB* DiTerrainMap::GetColorData()
    {
        return mDesc->mColorData;
    }

    ARGB* DiTerrainMap::GetColorData( uint32 x, uint32 y )
    {
        uint32 vertx,verty;
        GetVerticesNum(vertx,verty);

        DI_ASSERT (x >= 0 && x < vertx && y >= 0 && y < verty);
        return &mDesc->mColorData[y * vertx + x];
    }
    
    char* DiTerrainMap::GetCliffData()
    {
        return mDesc->mCliffData;
    }

    bool DiTerrainMap::CoverTerrain( const DiVec3& pos, float size )
    {
        float half = size/2;
        DiVec3 p;
        
        p = DiVec3(pos.x - half + GetTerrainWidth()/2, pos.y, pos.z - half + GetTerrainHeight()/2);
        if (CoverTerrain(p))
            return true;

        p = DiVec3(pos.x - half + GetTerrainWidth()/2, pos.y, pos.z + half + GetTerrainHeight()/2);
        if (CoverTerrain(p))
            return true;

        p = DiVec3(pos.x + half + GetTerrainWidth()/2, pos.y, pos.z - half + GetTerrainHeight()/2);
        if (CoverTerrain(p))
            return true;

        p = DiVec3(pos.x + half + GetTerrainWidth()/2, pos.y, pos.z + half + GetTerrainHeight()/2);
        if (CoverTerrain(p))
            return true;

        return false;
    }

    bool DiTerrainMap::CoverTerrain( const DiVec2& pos )
    {
        float width = GetTerrainWidth();
        float height = GetTerrainHeight();

        if (pos.x < 0 || pos.x > width || pos.y < 0 || pos.y > height)
        {
            return false;
        }
        return true;
    }

    bool DiTerrainMap::CoverTerrain( const DiVec3& pos )
    {
        float width = GetTerrainWidth();
        float height = GetTerrainHeight();

        DiVec2 terrainPos(pos.x+width/2,pos.z+height/2);
        return CoverTerrain(terrainPos);
    }

    float DiTerrainMap::GetTerrainWidth() const
    {
        float wholeSizeX = mDesc->mGridSize * mDesc->mSizeX * CHUNK_GRID_SIZE;
        return wholeSizeX;
    }

    float DiTerrainMap::GetTerrainHeight() const
    {
        float wholeSizeY = mDesc->mGridSize * mDesc->mSizeY * CHUNK_GRID_SIZE;
        return wholeSizeY;
    }

    void DiTerrainMap::GetNTAtPoint( uint32 x, uint32 y, DiVec3& normal, DiVec3& tangent)
    {
        DiVec3 here = GetPoint(x,y);
        DiVec2 hereUv(0,0);

        uint32 vertNumX,vertNumY;
        GetVerticesNum(vertNumX,vertNumY);

        DiPlane plane;
        DiVec3 adjacentPoints[8];
        DiVec3 cumulativeNormal = DiVec3::ZERO;

        GetPointOrNeighbour(x+1, y,   adjacentPoints[0]);
        GetPointOrNeighbour(x+1, y+1, adjacentPoints[1]);
        GetPointOrNeighbour(x,   y+1, adjacentPoints[2]);
        GetPointOrNeighbour(x-1, y+1, adjacentPoints[3]);
        GetPointOrNeighbour(x-1, y,   adjacentPoints[4]);
        GetPointOrNeighbour(x-1, y-1, adjacentPoints[5]);
        GetPointOrNeighbour(x,   y-1, adjacentPoints[6]);
        GetPointOrNeighbour(x+1, y-1, adjacentPoints[7]);
        for (int i = 0; i < 8; ++i)
        {
            plane.redefine(here, adjacentPoints[i], adjacentPoints[(i+1)%8]);
            cumulativeNormal += plane.normal;
        }
        cumulativeNormal.normalise();
        normal = -cumulativeNormal;

        bool xplus = false;
        bool yplus = true;

        DiVec2 Puv,Quv;

        DiVec3 P;
        if (x == 0)
        {
            P = GetPoint(x+1,y);
            Puv.x = 1;
            Puv.y = 0;
            xplus = true;
        }
        else
        {
            P = GetPoint(x-1,y);
            Puv.x = -1;
            Puv.y = 0;
        }

        DiVec3 Q;
        if (y >= vertNumY)
        {
            Q = GetPoint(x,y-1);
            Quv.x = 0;
            Quv.y = -1;
            yplus = false;
        }
        else
        {
            Q = GetPoint(x,y+1);
            Quv.x = 0;
            Quv.y = 1;
        }
        
        P = P - here;
        Q = Q - here;

        DiVec2 uv1,uv2;

        // calculate the tangents
        uv1 = DiVec2(Puv.x - hereUv.x, Puv.y - hereUv.y);
        uv2 = DiVec2(Quv.x - hereUv.x, Quv.y - hereUv.y);

        float tmp = 1.0f;
        if (!DiMath::RealEqual(DiMath::Abs(uv1.x*uv2.y - uv2.x*uv1.y),0))
        {
            tmp = 1.0f / (uv1.x*uv2.y - uv2.x*uv1.y);
        }

        tangent.x = uv2.y * P.x - uv1.y * Q.x;
        tangent.y = uv2.y * P.y - uv1.y * Q.y;
        tangent.z = uv2.y * P.z - uv1.y * Q.z;
        tangent = tmp * tangent;
        tangent.normalise();
    }

    DiVec3 DiTerrainMap::GetPoint( uint32 x, uint32 y )
    {
        DiVec3 pos;
        pos.x = x * mDesc->mGridSize - GetTerrainWidth() / 2;
        pos.y = GetHeightAtPoint(x,y);
        pos.z = y * mDesc->mGridSize - GetTerrainHeight() / 2;
        return pos;
    }

    void DiTerrainMap::GetNormalTangent( uint16 trunkIDx, uint16 trunkIDy, uint32 vertid, DiVec3& n, DiVec3& t )
    {
        uint32 realid = GetRealVertId(trunkIDx,trunkIDy,vertid);

        uint32 vertNumX,vertNumY;
        GetVerticesNum(vertNumX,vertNumY);

        DI_ASSERT(vertid < vertNumX*vertNumY);

        uint32 vertx = realid % vertNumX;
        uint32 verty = realid / vertNumX;

        GetNTAtPoint(vertx,verty,n,t);
    }

    bool DiTerrainMap::AddTexture( int ID,const DiString& texBaseName )
    {
        if(!GetTextureTable().contains(ID))
        {
            GetTextureTable()[ID] = texBaseName;
            return true;
        }
        return false;
    }

    void DiTerrainMap::AddTexture( const DiSet<DiString>& bs )
    {
        DiSet<DiString>::const_iterator it;
        DiSet<DiString>::const_iterator itEnd = bs.end();
        for (it = bs.begin(); it != itEnd; ++it)
        {
            AddTexture(*it);
        }
    }

    int DiTerrainMap::AddTexture( const DiString& texBaseName )
    {
        if (mDesc->mTextureTable.empty())
            mDesc->mTextureTable[0] = texBaseName;

        DiTerrainDesc::TextureTable::iterator it;
        DiTerrainDesc::TextureTable::iterator itEnd = mDesc->mTextureTable.end();
        int maxid = -1000;
        for (it = mDesc->mTextureTable.begin(); it != itEnd; ++it)
        {
            if (it->second == texBaseName)
                return it->first;
            else
            {
                if (it->first > maxid)
                    maxid = it->first;
            }
        }
        
        mDesc->mTextureTable[maxid+1] = texBaseName;

        return maxid+1;
    }

    DiMaterialPtr DiTerrainMap::GenerateMaterial( const DiString& layer0,const DiString& layer1 )
    {
        DiString matName;
        matName.Format("_ter_mat_%s_%s",layer0.c_str(),layer1.c_str());

        DiMaterialPtr mat = DiAssetManager::GetInstance().CreateOrReplaceAsset<DiMaterial>(matName);

        mat->LoadShader(DiMaterialDefine::TERRAIN_VERTEX_SHADER,
                        DiMaterialDefine::TERRAIN_PIXEL_SHADER);

        DiShaderParameter* params = mat->GetShaderParameter();
    
        DiVec4 worldSize(GetGridSize(),GetTextureScale(),1,1);
        params->WriteFloat4("v_WorldSizes",worldSize);

        DiString texl0Dif;
        DiString texl0Norm;

        DiString texl1Dif;
        DiString texl1Norm;

        if (layer0.empty())
        {
            texl0Dif = "_default.dds";
            texl0Norm = "_default.dds";
        }
        else
        {
            texl0Dif = layer0 + "_d.dds";
            texl0Norm = layer0 + "_n.dds";
        }

        if (layer1.empty())
        {
            texl1Dif = "_default.dds";
            texl1Norm = "_default.dds";
        }
        else
        {
            texl1Dif = layer1 + "_d.dds";
            texl1Norm = layer1 + "_n.dds";
        }

        DiTexturePtr texture;

        texture = params->WriteTexture2D("diffuseMap_0", texl0Dif);
        texture->SetFilter(FILTER_BILINEAR);

        texture = params->WriteTexture2D("diffuseMap_1", texl1Dif);
        texture->SetFilter(FILTER_BILINEAR);

        texture = params->WriteTexture2D("normalMap_0", texl0Norm);
        texture->SetFilter(FILTER_BILINEAR);

        texture = params->WriteTexture2D("normalMap_1", texl1Norm);
        texture->SetFilter(FILTER_BILINEAR);

        mat->SetCullMode(CULL_CCW);

        return mat;
    }

    bool DiTerrainMap::RayIntersects( const DiRay& ray, DiVec3& out )
    {
        DiAABB aabb = mRoot->GetBounds();
        float result = 0;
        bool testok = false;

        float d1, d2;
        if (!DiMath::intersects(ray, aabb, &d1, &d2))
        {
            return false;
        }

        DiVec3 start = ray.getPoint(d1) + GetTerrainWidth()/2;
        DiVec3 end = ray.getPoint(d2) + GetTerrainHeight()/2;;

        float xstep = ray.getDirection().x * GetGridSize();
        float zstep = ray.getDirection().z * GetGridSize();

        if (DiMath::Abs(xstep) < DiMath::Abs(zstep))
        {
            int zinc = ray.getDirection().z > 0 ? +1 : -1;
            float zstart = start.z / GetGridSize();
            int zbegin = DiMath::IFloor(zstart) - zinc;
            int zend = DiMath::IFloor(end.z / GetGridSize()) + zinc;
            float xfactor = xstep / zstep;
            float xinc = zinc * xfactor;
            float xiter = (start.x / GetGridSize())- (zstart - zbegin) * xfactor;
            DI_ASSERT((zend - zbegin) * zinc > 0);
            DI_ASSERT(DiMath::Abs(xinc) <= 1);

            for (int zi = zbegin; zi != zend; zi += zinc, xiter += xinc)
            {
                int xi = DiMath::IFloor(xiter);
                bool r = false;
                r = QuadRayIntersects(xi+0, zi, ray, result);
                if (r)                               
                {
                    testok = true;
                    break;
                }
                r = QuadRayIntersects(xi-1, zi, ray, result);
                if (r)
                {
                    testok = true;
                    break;
                }
                r = QuadRayIntersects(xi+1, zi, ray, result);
                if (r)
                {
                    testok = true;
                    break;
                }
            }
        }
        else
        {
            int xinc = ray.getDirection().x > 0 ? +1 : -1;
            float xstart = start.x  / GetGridSize();
            int xbegin = DiMath::IFloor(xstart) - xinc;
            int xend = DiMath::IFloor(end.x / GetGridSize()) + xinc;
            float zfactor = zstep / xstep;
            float zinc = xinc * zfactor;
            float ziter = (start.z / GetGridSize()) - (xstart - xbegin) * zfactor;
            assert((xend - xbegin) * xinc > 0);
            assert(DiMath::Abs(zinc) <= 1);
            for (int xi = xbegin; xi != xend; xi += xinc, ziter += zinc)
            {
                int zi = DiMath::IFloor(ziter);
                bool r = false;
                r = QuadRayIntersects(xi, zi+0, ray, result);
                if (r)                                     
                {
                    testok = true;
                    break;                                 
                }                                         
                r = QuadRayIntersects(xi, zi-1, ray, result);
                if (r)                                     
                {                                         
                    testok = true;
                    break;                                 
                }                                         
                r = QuadRayIntersects(xi, zi+1, ray, result);
                if (r)
                {
                    testok = true;
                    break;
                }
            }
        }

        out = ray.getPoint(result);
        return testok;
    }

    DiVec2 DiTerrainMap::GetWorldSize() const
    {
        DiVec2 size;
        size.x = mDesc->mGridSize * mDesc->mSizeX * CHUNK_GRID_SIZE;
        size.y = mDesc->mGridSize * mDesc->mSizeY * CHUNK_GRID_SIZE;;
        return size;
    }

    DiIntVec2 DiTerrainMap::GetChunkSize() const
    {
        return DiIntVec2(mDesc->mSizeX,mDesc->mSizeY);
    }

    DiVec3 DiTerrainMap::GetPosition( uint16 trunkIDx, uint16 trunkIDy, uint32 vertid )
    {
        DiVec3 position;

        BYTE tileIDx,tileIDy;
        tileIDx = (BYTE)(vertid % ((uint32)CHUNK_GRID_SIZE + 1));
        tileIDy = (BYTE)(vertid / ((uint32)CHUNK_GRID_SIZE + 1));

        position.x = (trunkIDx * CHUNK_GRID_SIZE + tileIDx) * mDesc->mGridSize - GetTerrainWidth() / 2;
        position.y = GetHeight(trunkIDx,trunkIDy,vertid);
        position.z = (trunkIDy * CHUNK_GRID_SIZE + tileIDy) * mDesc->mGridSize - GetTerrainWidth() / 2;

        return position;
    }

    void DiTerrainMap::AddToBatchGroup(DiRenderBatchGroup* bg)
    {
        for (auto it = mVisibles.begin(); it != mVisibles.end(); ++it)
        {
            DiTerrainBatchBase* batch = static_cast<DiTerrainBatchBase*>(*it);
            batch->UpdateMaterialParams();
            bg->AddRenderUnit(batch);
        }
    }

    void DiTerrainMap::Update(DiCamera* camera)
    {
        mVisibles.clear();

        DiVector<int> visibleChunk;
        mRoot->WalkQuadTree(camera, visibleChunk);

        size_t num = visibleChunk.size();
        for (size_t i = 0; i<num; i++)
            mChunks[visibleChunk[i]]->GetBatches(mVisibles);

        if (mWaterMap)
        {
            for (size_t i = 0; i<num; i++)
            {
                mWaterMap->GetChunk(visibleChunk[i])->UpdateMaterial();
                mVisibles.push_back(mWaterMap->GetChunk(visibleChunk[i]));
            }
        }

        if (mFoliageMap)
        {
            mFoliageMap->Update();
            for (size_t i = 0; i<num; i++)
                mFoliageMap->GetChunk(visibleChunk[i])->GetBatches(mVisibles);
        }
    }

    const DiAABB& DiTerrainMap::GetBoundingBox( void ) const
    {
        return DiAABB::BOX_INFINITE;
    }

    bool DiTerrainMap::QuadRayIntersects( uint32 x, uint32 z, const DiRay& ray, float& outPos )
    {
        DiVec3 v1 = GetPoint(x,z);
        DiVec3 v2 = GetPoint(x+1,z);
        DiVec3 v3 = GetPoint(x,z+1);
        DiVec3 v4 = GetPoint(x+1,z+1);

        DiPlane p1, p2;
        bool oddRow = false;
        if (z % 2 && x % 2)
        {
            /* 
            3---4
            | / |
            1---2
            */
            p1.redefine(v1, v2, v4);
            p2.redefine(v1, v4, v3);
        }
        else
        {
            /* 
            3---4
            | \ |
            1---2
            */
            p1.redefine(v2, v4, v3);
            p2.redefine(v1, v2, v3);
            oddRow = true;
        }

        std::pair<bool, float> planeInt = ray.intersects(p1);
        if (planeInt.first)
        {
            DiVec3 where = ray.getPoint(planeInt.second);
            DiVec3 rel = where - v1;
            if (rel.x >= -1 && rel.x <= GetGridSize()+1 && rel.z >= -1 && rel.z <= GetGridSize()+1
                && ((rel.x >= rel.z && !oddRow) || (rel.x >= (1 - rel.z) && oddRow)))
            {
                outPos = planeInt.second;
                return true;
            }
        }
        planeInt = ray.intersects(p2);
        if (planeInt.first)
        {
            DiVec3 where = ray.getPoint(planeInt.second);
            DiVec3 rel = where - v1;
            if (rel.x >= -1 && rel.x <= GetGridSize()+1 && rel.z >= -1 && rel.z <= GetGridSize()+1
                && ((rel.x <= rel.z && !oddRow) || (rel.x <= (1 - rel.z) && oddRow))) 
            {
                outPos = planeInt.second;
                return true;
            }
        }

        return false;
    }

    void DiTerrainMap::GetPointOrNeighbour( int x, int y, DiVec3& out )
    {
        uint32 vx,vy;
        GetVerticesNum(vx,vy);
        x = DiMath::Min(x, (int)vx - 1);
        y = DiMath::Min(y, (int)vy - 1);
        x = DiMath::Max(x, 0);
        y = DiMath::Max(y, 0);
        out = GetPoint(x, y);
    }

    void DiTerrainMap::UpdateMaxMinHeight( float newheight )
    {
        if (newheight > mMaxHeight)
        {
            mMaxHeight = newheight;
        }
        if (newheight < mMinHeight)
        {
            mMinHeight = newheight;
        }
    }

    DiMap<DiString,int> DiTerrainMap::GetTextureUsages( int layer )
    {
        DiMap<DiString,int> usages;

        DiVector<DiTerrainChunk*>::iterator it;
        DiVector<DiTerrainChunk*>::iterator itEnd = mChunks.end();
        for (it = mChunks.begin(); it != itEnd; ++it)
        {
            DiTerrainChunk* chunk = *it;

            if (chunk->mBatches.empty())
            {
                continue;
            }
            else
            {
                for (size_t i=0; i<chunk->mBatches.size(); i++)
                {
                    DiString name = chunk->mBatches[i]->DemiureAlias[layer];
                    if (!usages.contains(name))
                    {
                        usages[name] = chunk->mBatches[i]->mPrimitiveCount/2;
                    }
                    else
                    {
                        usages[name] += chunk->mBatches[i]->mPrimitiveCount/2;
                    }
                }
            }
        }

        return usages;
    }

    void DiTerrainMap::ShowLayer( int layerID, bool vis )
    {
        DiPair<DiString,DiString> marco;
        if (layerID == 0)
            marco.first = "_SHOW_LAYER_0";
        else
            marco.first = "_SHOW_LAYER_1";
        
        marco.second = vis?"1":"0";

        for (auto it = mMaterialTable.begin(); it != mMaterialTable.end(); ++it)
        {
            (it->second)->RecompileShader(SHADER_PIXEL,marco);
        }
    }

    int DiTerrainMap::GetTextureID( const DiString& names )
    {
        for (auto it = mDesc->mTextureTable.begin(); it != mDesc->mTextureTable.end(); ++it)
        {
            if (it->second == names)
                return it->first;
        }
        return -1;
    }

    void DiTerrainMap::LoadWaterMap()
    {
        // water data
        mWaterMap = DI_NEW DiWaterMap(this);
        mWaterMap->Load();

        // create chunks
        for (uint16 x = 0; x < mDesc->mSizeX; ++x)
        {
            for (uint16 y = 0; y < mDesc->mSizeY; ++y)
            {
                mWaterMap->AddChunk(x,y);
            }
        }
    }

    void DiTerrainMap::LoadFoliageMap()
    {
        mFoliageMap = DI_NEW DiFoliageMap(this);
        mFoliageMap->Load();

        mFoliageMap->CreateLayerFromDesc();

        // create chunks
        for (uint16 x = 0; x < mDesc->mSizeX; ++x)
        {
            for (uint16 y = 0; y < mDesc->mSizeY; ++y)
            {
                mFoliageMap->AddChunk(x,y);
            }
        }
    }

    DiVec2 DiTerrainMap::GetChunkCenterPos( uint32 idx, uint32 idy )
    {
        DiVec2 pos(0,0);

        pos.x = idx * mDesc->mGridSize * CHUNK_GRID_SIZE;
        pos.y = idy * mDesc->mGridSize * CHUNK_GRID_SIZE;

        float wholeSizeX = mDesc->mGridSize * mDesc->mSizeX * CHUNK_GRID_SIZE;
        float wholeSizeY = mDesc->mGridSize * mDesc->mSizeY * CHUNK_GRID_SIZE;

        pos.x -= wholeSizeX/2;
        pos.y -= wholeSizeY/2;

        return pos;
    }

    DiString& DiTerrainMap::GetType()
    {
        static DiString type = "TerrainMap";
        return type;
    }
}