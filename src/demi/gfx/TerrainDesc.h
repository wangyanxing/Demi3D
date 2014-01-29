
/**************************************************************************
    File:       TerrainDesc.h
    Creator:    demiwangya
**************************************************************************/

#pragma once

namespace Demi
{
    struct DI_GFX_API DiTerrainLayerDesc
    {
        DiTerrainLayerDesc()
        {
            DemiureId    = NULL;
        }

        BYTE*    DemiureId;
    };

    struct DI_GFX_API DiFoliageLayerDesc
    {
        DiFoliageLayerDesc();

        void            Load(DiDataStreamPtr ds);

        void            Save(DiDataStreamPtr ds);

        void            ClearDensityMap();

        DiString        DemiureName;

        BYTE            mRenderTechnique;

        float           mDensity;

        bool            mAnimate;

        bool            mLighting;

        float           mAnimMag;

        float           mAnimSpeed;

        float           mAnimFreq;

        float           mMinWidth, mMaxWidth;

        float           mMinHeight, mMaxHeight;

        ARGB            mColor;

        BYTE            mDensityMap[FOLIAGE_DENSITY_MAP_SIZE*FOLIAGE_DENSITY_MAP_SIZE];
    };
    
    struct DiTerrainEntity 
    {
        DiModel*    pkModel;
        DiString    kName;    
    };
                

    class DI_GFX_API DiTerrainDesc
    {
    public:

        DiTerrainDesc();

        ~DiTerrainDesc();

    public:

        bool                CheckValid();

        void                Release();

        uint32              GetVertNum();

        uint32              GetGridNum();
        
        uint32              GetCliffVertNum();
        
        uint32              GetCliffGridNum();
        
        uint32              GetCliffGridXNum();
        
        uint32              GetCliffGridYNum();

    public:

        float               mGridSize;    

        uint16              mSizeX;        

        uint16              mSizeY;        

        float*              mHeightData;    

        float               DemiureScale;

        uint32*             mColorData;    

        uint32              mCliffSpace;    

        char*               mCliffData;    

        BYTE*               mWaterMap;    

        float*              mWaterHeightMap;

        typedef DiMap<int,DiString>    TextureTable;

        TextureTable        DemiureTable;

        DiTerrainLayerDesc  mLayers[TERRAIN_LAYER_NUM];

        typedef DiVector<DiFoliageLayerDesc*> FoliageLayers;

        FoliageLayers       mFoliageLayers;
    };
}