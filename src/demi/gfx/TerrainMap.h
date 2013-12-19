
/********************************************************************
    File:       TerrainMap.h
    Creator:    demiwangya
*********************************************************************/

#pragma once

#include "TerrainDesc.h"
#include "Material.h"
#include "TransformUnit.h"

namespace Demi
{
    typedef shared_ptr<DiTerrainMap> DiTerrainPtr;

    class DI_GFX_API DiTerrainMap : public DiTransformUnit
    {
    public:

        DiTerrainMap(DiSceneManager* sm);

        virtual                     ~DiTerrainMap();

        friend class DiTerrainChunk;

    public:

        bool                        Load(const DiString& sceneName);

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

        ARGB*                       GetColorData();

        ARGB*                       GetColorData(uint32 x, uint32 y);

        BYTE*                       GetTextureID(uint16 layer) { return mDesc->mLayers[layer].DemiureId; }

        char*                       GetCliffData();

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

        DiSet<uint32>&              GetHidedTile() { return mHidedTile; }

        bool                        CoverTerrain(const DiVec3& pos, float size);

        bool                        CoverTerrain(const DiVec2& pos);

        bool                        CoverTerrain(const DiVec3& pos);

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

        float                       GetTextureScale() const { return mDesc->DemiureScale; }

        void                        SetTextureScale(float v) const{mDesc->DemiureScale=v;}

        DiTerrainDesc::TextureTable&GetTextureTable() { return mDesc->DemiureTable; }

        DiMap<DiString,int>         GetTextureUsages(int layer);

        void                        ShowLayer(int layerID, bool vis);

        DiTerrainDesc*              GetDesc() { return mDesc; }

        void                        LoadWaterMap();

        void                        LoadFoliageMap();

        DiSceneManager*             GetSceneManager() const { return mCreator; }

        virtual DiString&           GetType();
                
    public:

        void                        CullingUpdate(DiRenderBatchGroup*,DiCamera*);

        void                        AddToBatchGroup(DiRenderBatchGroup* bg);

        void                        Update(DiCamera* camera);

        const DiAABB&               GetBoundingBox(void) const;

    protected:

        void                        Cull(DiCamera*);

        void                        CreateVertexDecl();

        ARGB                        GetColor(uint32 vertid);

        ARGB                        GetColor(uint16 trunkIDx, uint16 trunkIDy, uint32 vertid);

        float                       GetHeight(uint16 trunkIDx, uint16 trunkIDy, uint32 vertid);

        DiVec3                      GetPosition(uint16 trunkIDx, uint16 trunkIDy, uint32 vertid);

        void                        GetNormalTangent(uint16 trunkIDx, uint16 trunkIDy, uint32 vertid, DiVec3& n, DiVec3& t);

        uint32                      GetRealVertId(uint16 trunkIDx, uint16 trunkIDy, uint32 vertid);

        uint32                      GetRealGridId(uint16 trunkIDx, uint16 trunkIDy, uint32 gridID);

        DiMaterialPtr               GetMaterial(const DiPair<BYTE,BYTE>& textureid);

        DiMaterialPtr               GenerateMaterial(const DiString& layer0,const DiString& layer1);
        
        void                        GetPointOrNeighbour(int x, int y, DiVec3& out);

    protected:

        typedef DiMap<DiPair<BYTE,BYTE>,DiMaterialPtr> MaterialTable;

        MaterialTable               mMaterialTable;

        DiVertexDeclaration*        mVertexDecl;

        DiVertexBuffer*             mVertexBuffer;

        DiVector<DiRenderUnit*>     mVisibles;

        DiVector<DiTerrainChunk*>   mChunks;

        DiTerrainDesc*              mDesc;
        
        DiWaterMap*                 mWaterMap;

        DiFoliageMap*               mFoliageMap;
        
        float                       mMaxHeight;
        
        float                       mMinHeight;

        DiQuadNode*                 mRoot;

        DiSet<uint32>               mHidedTile;

        DiSceneManager*             mCreator;
    };
}