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

#ifndef DiFoliageMap_h__
#define DiFoliageMap_h__

#include "K2Prerequisites.h"

namespace Demi
{
    class DEMI_K2_API DiFoliageMap
    {
    public:
        friend class DiFoliageChunk;

        DiFoliageMap(DiTerrain* terrainmap);

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

        DiTerrain*                  GetTerrainMap(){return mTerrainMap;}

        void                        Update();

        DiVec3                      GetWindDir() const { return mWindDir; }

        void                        SetWindDir(Demi::DiVec3 val) { mWindDir = val; }

    protected:

        void                        DeleteChunkLayer( DiFoliageLayer* layer );

    protected:

        DiTerrain*                  mTerrainMap;

        DiVector<DiFoliageChunk*>   mChunks;

        LayerList                   mLayers;

        DiRandomTable*              mTable;

        DiTimer                     mWindTimer;

        unsigned long               mLastTime;

        DiVec3                      mWindDir;

    };
}

#endif
