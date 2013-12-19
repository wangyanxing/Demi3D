
/********************************************************************
    File:       WaterChunk.h
    Creator:    demiwangya
*********************************************************************/

#pragma once

#include "BatchBase.h"

namespace Demi
{
    class DI_GFX_API DiWaterChunk : public DiTerrainBatchBase
    {
    public:

        DiWaterChunk(uint16 x, uint16 y, DiWaterMap* parent);

        virtual         ~DiWaterChunk();

    public:

        void            BuildBuffers();

        void            GetWorldTransform(DiMat4* xform) const;

        void            UpdateMaterial();

        float           GetHeight() const { return mHeight; }

        void            SetHeight(float val);

    protected:

        DiWaterMap*     mParent;

        DiVertexBuffer* mVertexBuffer;

        DiIntVec2       mChunkID;

        float           mHeight;
    };
}