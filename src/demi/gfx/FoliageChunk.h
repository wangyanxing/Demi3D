
/********************************************************************
    File:       FoliageChunk.h
    Creator:    demiwangya
*********************************************************************/

#pragma once

#include "BatchBase.h"

namespace Demi
{
    class DI_GFX_API DiFoliageLayerBatch : public DiTerrainBatchBase
    {
    public:

        DiFoliageLayerBatch(DiFoliageChunk* parent, DiFoliageLayer* layer);

        virtual             ~DiFoliageLayerBatch();

        void                GetWorldTransform(DiMat4* xform) const;

        void                Load();

    protected:

        void                GenerateGrassQuad(const float *grassPositions, unsigned int grassCount);

        void                GenerateGrassCrossQuads(const float *grassPositions, unsigned int grassCount);

        void                GenerateGrassSprite(const float *grassPositions, unsigned int grassCount);

        uint32              PopulateGrassList(float *posBuff, unsigned int grassCount);

    protected:

        DiFoliageChunk*     mParent;

        DiFoliageLayer*     mLayer;
    };

    //////////////////////////////////////////////////////////////////////////

    class DI_GFX_API DiFoliageChunk
    {
    public:

        friend class DiFoliageLayerBatch;

        DiFoliageChunk(uint16 x, uint16 y, DiFoliageMap* parent);

        virtual             ~DiFoliageChunk();

    public:
        
        void                BuildBatches();

        void                CleanBatches();

        void                DeleteBatch(DiFoliageLayer* l);

        void                GetBatches(DiVector<DiRenderUnit*>& visible);

        DiRandomTable*      GetRandomTable();

        void                RebuildBatch(DiFoliageLayer* layer);

    protected:

        typedef DiMap<unsigned long,DiFoliageLayerBatch*> LayerBatches;

        LayerBatches        mLayerBatches;

        DiFoliageMap*       mParent;

        DiIntVec2           mChunkID;
    };
}