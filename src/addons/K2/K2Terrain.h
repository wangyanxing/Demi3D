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

#ifndef DiTerrainMap_h__
#define DiTerrainMap_h__

#include "K2Prerequisites.h"
#include "K2TerrainDesc.h"
#include "K2MapLoader.h"
#include "K2PathFinder.h"

#include "Material.h"
#include "TransformUnit.h"

namespace Demi
{
    class DEMI_K2_API DiTerrain : public DiTransformUnit
    {
    public:

        DiTerrain();

        virtual                     ~DiTerrain();

        friend class DiTerrainChunk;

    public:

        bool                        Load(DiTerrainDescPtr desc);

        void                        Unload();

        float                       GetHeightAtPoint(uint32 x, uint32 y);

        bool                        GetHeight(float worldX, float worldZ, float& outHeight);

        bool                        RayIntersects(const DiRay& ray, DiVec3& out);

        bool                        QuadRayIntersects(uint32 x, uint32 y, const DiRay& ray, float& outPos);

        DiVec3                      GetPoint(uint32 x, uint32 y);

        void                        GetNTAtPoint(uint32 x, uint32 y, DiVec3& normal, DiVec3& tangent);

        DiColor                     GetColorAtPoint(uint32 x, uint32 y);

        float*                      GetHeightData();

        float*                      GetHeightData(uint32 x, uint32 y);

        DiVec2                      GetWorldSize() const;

        DiIntVec2                   GetChunkSize() const;

        uint32*                     GetColorData();

        uint32*                     GetColorData(uint32 x, uint32 y);

        uint8*                      GetTileCliffData() const { return mDesc->mTileCliffMap->GetBuffer(); }

        K2TileLayer*                GetTextureID(uint16 layer) { return mDesc->mTextureIDMap->GetBuffer(layer); }

        DiWaterMap*                 GetWaterMap(){ return mWaterMap; }

        DiFoliageMap*               GetFoliageMap(){ return mFoliageMap; }
        
        void                        GetVerticesNum(uint32& outx, uint32& outy);

        DiIntVec2                   GetVerticesNum();

        DiIntVec2                   GetVertexPos(const DiVec3& WSpos);

        void                        DirtyRect(DiRect rect);

        void                        DirtyTile(uint32 x, uint32 y);

        DiTerrainChunk*             GetChunk(uint32 idx, uint32 idy);

        DiVec2                      GetChunkCenterPos(uint32 idx, uint32 idy);

        DiTerrainChunk*             GetChunk(uint32 id);

        bool                        CoverTerrain(const DiVec2& pos);

        float                       GetTerrainWidth() const;

        float                       GetTerrainHeight() const;

        bool                        AddTexture(int ID,const DiString& texBaseName);

        int                         AddTexture(const DiString& texBaseName);

        void                        AddTexture(const DiSet<DiString>& names);

        int                         GetTextureID(const DiString& names);

        float                       GetMaxHeight() const { return mMaxHeight; }

        float                       GetMinHeight() const { return mMinHeight; }

        void                        UpdateMaxMinHeight(float newheight);

        float                       GetGridSize() const { return mDesc->mGridSize; }

        float                       GetTextureScale() const { return mDesc->mTextureScale; }

        void                        SetTextureScale(float v) const{mDesc->mTextureScale=v;}

        DiTerrainDesc::TextureTable&GetTextureTable() { return mDesc->mTextureTable; }

        DiMap<DiString,int>         GetTextureUsages(int layer);

        void                        ShowLayer(int layerID, bool vis);

        DiTerrainDescPtr            GetDesc() { return mDesc; }

        void                        LoadWaterMap();

        void                        LoadFoliageMap();

        virtual DiString&           GetType();

        HeavyPathFinder&            GetPathFinder() { return mPathFinder; }

    public:

        void                        AddToBatchGroup(DiRenderBatchGroup* bg);

        void                        Update(DiCamera* camera);

        const DiAABB&               GetBoundingBox(void) const;

    protected:

        void                        Render();

        void                        Cull(DiCamera*);

        void                        CreateVertexDecl();

        uint32                      GetColor(uint32 vertid);

        uint32                      GetColor(uint16 trunkIDx, uint16 trunkIDy, uint32 vertid);

        float                       GetHeight(uint16 trunkIDx, uint16 trunkIDy, uint32 vertid);

        DiVec3                      GetPosition(uint16 trunkIDx, uint16 trunkIDy, uint32 vertid);

        void                        GetNormalTangent(uint16 trunkIDx, uint16 trunkIDy, uint32 vertid, DiVec3& n, DiVec3& t);

        uint32                      GetRealVertId(uint16 trunkIDx, uint16 trunkIDy, uint32 vertid);

        uint32                      GetRealGridId(uint16 trunkIDx, uint16 trunkIDy, uint32 gridID);

        DiMaterialPtr               GetMaterial(const K2TerrainTexture& textureid);

        DiMaterialPtr               GenerateMaterial(const DiString matName, const DiString& dif0,const DiString& dif1,
                                        const DiString& norm0, const DiString& norm1);
        
        void                        GetPointOrNeighbour(int x, int y, DiVec3& out);

    protected:

        typedef DiMap<K2TerrainTexture, DiMaterialPtr> MaterialTable;

        MaterialTable               mMaterialTable;

        DiVertexDeclaration*        mVertexDecl;

        DiVertexBuffer*             mVertexBuffer;

        DiVector<DiRenderUnit*>     mVisibles;

        DiVector<DiTerrainChunk*>   mChunks;

        DiTerrainDescPtr            mDesc;
        
        DiWaterMap*                 mWaterMap;

        DiFoliageMap*               mFoliageMap;
        
        float                       mMaxHeight;
        
        float                       mMinHeight;

        DiQuadNode*                 mRoot;

        HeavyPathFinder             mPathFinder;
    };
}

#endif
