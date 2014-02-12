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

#ifndef DiWaterChunk_h__
#define DiWaterChunk_h__


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

#endif
