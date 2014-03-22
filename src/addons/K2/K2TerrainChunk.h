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

#ifndef DiTerrainChunk_h__
#define DiTerrainChunk_h__

#include "K2Prerequisites.h"
#include "K2TerrainDesc.h"
#include "K2BatchBase.h"

namespace Demi
{
    class DEMI_K2_API DiTerrainBatch : public DiTerrainBatchBase
    {
    public:

        DiTerrainBatch(DiTerrainChunk* parent);

        virtual         ~DiTerrainBatch();

    public:

        void            GetWorldTransform(DiMat4* xform) const;

        void            UpdateMaterialParams();

        DiTerrainChunk* mParent;

        DiString        mDiffuseTexture[TERRAIN_LAYER_NUM];

        DiString        mNormalTexture[TERRAIN_LAYER_NUM];
    };

    //////////////////////////////////////////////////////////////////////////

    class DEMI_K2_API DiTerrainChunk : public DiBase
    {
    public:

        DiTerrainChunk(uint16 x, uint16 y, DiTerrain* parent);

        virtual                ~DiTerrainChunk();

    public:

        friend class DiTerrain;
        friend class DiTerrainBatch;

        typedef DiVector<DiTerrainBatch*> Batches;

        void                Load();

        void                Unload();

        int                 GetChunkID();

        void                AddToBatchGroup(DiRenderBatchGroup* bg);

        void                CorrectChunkPosition();

        void                NotifyVerticesModified(uint32 vertexBegin, uint32 vertexEnd);

        void                NotifyVerticesModified(const DiRect& vertRect);

        DiFloatRect         GetRealRect();

        DiRect              GetGridRect();

        DiIntVec2           TransformRect(const DiFloatRect& rect);

        DiQuadNode*         GetQuadNode() const { return mQuadNode; }

        void                SetQuadNode(DiQuadNode* val) { mQuadNode = val; }

        void                ResetBounds();

        void                GetBatches(DiVector<DiRenderUnit*>& visible);

        const DiMat4&       GetTransform(void) const;

        void                SetPosition(const DiVec3& pos);

        void                BuildBatches();

        void                BuildVertexBuffer();

        void                BuildIndexBuffer();

    protected:

        DiTerrain*          mParent;

        DiVertexBuffer*     mVertexBuffer;

        DiIndexBuffer*      mIndexBuffer;

        uint16              mChunkIDX;

        uint16              mChunkIDY;

        Batches             mBatches;

        DiQuadNode*         mQuadNode;
        
        DiMat4              mChunkTransform;
    };
}

#endif
