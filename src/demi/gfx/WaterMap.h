


#ifndef DiWaterMap_h__
#define DiWaterMap_h__


#include "Texture.h"
#include "Material.h"
#include "VertexDeclaration.h"
#include "IndexBuffer.h"

namespace Demi
{
    class DI_GFX_API DiWaterMap
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
