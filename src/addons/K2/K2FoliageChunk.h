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

#ifndef DiFoliageChunk_h__
#define DiFoliageChunk_h__

#include "K2Prerequisites.h"
#include "K2BatchBase.h"

namespace Demi
{
    class DEMI_K2_API DiFoliageLayerBatch : public DiTerrainBatchBase
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

    class DEMI_K2_API DiFoliageChunk
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

#endif
