
/********************************************************************
    File:       FoliageMap.h
    Creator:    demiwangya
*********************************************************************/

#pragma once

namespace Demi
{
    class DI_GFX_API DiFoliageMap
    {
    public:
        friend class DiFoliageChunk;

        DiFoliageMap(DiTerrainMap* terrainmap);

        virtual                     ~DiFoliageMap();

        typedef DiVector<DiFoliageLayer*> LayerList;

    public:

        void                        Load();

        void                        Unload();

        void                        AddChunk(uint16 x, uint16 y);

        DiFoliageChunk*             GetChunk(int id);

        DiFoliageLayer*             GetLayer(size_t id);

        int                         GetLayerNums(){return (int)mLayers.size();}

        void                        CreateLayerFromDesc();

        DiFoliageLayer*             AddNewLayer( DiFoliageLayerDesc* desc);

        void                        DeleteLayer( int layerID );

        DiTerrainMap*               GetTerrainMap(){return mTerrainMap;}

        void                        Update();

        DiVec3                      GetWindDir() const { return mWindDir; }

        void                        SetWindDir(Demi::DiVec3 val) { mWindDir = val; }

    protected:

        void                        DeleteChunkLayer( DiFoliageLayer* layer );

    protected:

        DiTerrainMap*               mTerrainMap;

        DiVector<DiFoliageChunk*>   mChunks;

        LayerList                   mLayers;

        DiRandomTable*              mTable;

        DiTimer                     mWindTimer;

        unsigned long               mLastTime;

        DiVec3                      mWindDir;

    };
}