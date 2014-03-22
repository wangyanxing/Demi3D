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

#ifndef DiWaterMap_h__
#define DiWaterMap_h__

#include "K2Prerequisites.h"
#include "Texture.h"
#include "Material.h"
#include "VertexDeclaration.h"
#include "IndexBuffer.h"

namespace Demi
{
    class DEMI_K2_API DiWaterMap
    {
    public:

        friend class DiWaterChunk;

        DiWaterMap(DiTerrainMap* terrainmap);

        virtual                 ~DiWaterMap();

    public:

        void                    Load();

        void                    Unload();
        
        void                    AddChunk(uint16 x, uint16 y);

        DiWaterChunk*           GetChunk(int id);

        void                    ClearWaterMap();

        DiMaterialPtr           GetWaterMaterial() const { return mWaterMaterial; }

        void                    UpdateWaterTexture();

        void                    SetChunkHeight(int id, float height);

    protected:

        void                    CreateMaterial();

        void                    CreateCommonResource();

    protected:

        DiTerrainMap*           mTerrainMap;

        DiTexturePtr            mWaterMapTexture;

        DiMaterialPtr           mWaterMaterial;
        
        DiVertexDeclaration*    mCommonDecl;

        DiIndexBuffer*          mIndexBuffer;

        DiVector<DiWaterChunk*> mChunks;
    };
}


#endif
