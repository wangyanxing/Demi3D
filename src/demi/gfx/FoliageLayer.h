
/********************************************************************
    File:       FoliageLayer.h
    Creator:    demiwangya
*********************************************************************/

#pragma once

#include "Material.h"
#include "TerrainDesc.h"
#include "PropertyMaps.h"

namespace Demi
{
    enum GrassTechnique
    {
        GRASSTECH_QUAD,
        GRASSTECH_CROSSQUADS,
        GRASSTECH_SPRITE
    };

    enum FadeTechnique
    {
        FADETECH_ALPHA,
        FADETECH_GROW,
        FADETECH_ALPHAGROW
    };

    //////////////////////////////////////////////////////////////////////////

    class DI_GFX_API DiFoliageLayer
    {
    public:

        DiFoliageLayer(DiFoliageMap* map, DiFoliageLayerDesc* desc);

        ~DiFoliageLayer();

        friend class DiFoliageLayerBatch;
        friend class DiFoliageMap;

    public:

        void                SetColor(const DiColor& c);

        void                SetMinimumSize(float width, float height);

        void                SetMaximumSize(float width, float height);

        void                SetDensity(float density);

        void                SetRenderTechnique(GrassTechnique style, BOOL blendBase = false);

        void                SetAnimationEnabled(BOOL enabled);

        void                SetLightingEnabled(BOOL enabled);

        void                SetSwayLength(float mag);

        void                SetSwaySpeed(float speed);

        void                SetSwayDistribution(float freq);

        DiMaterialPtr       GetMaterial() const { return mMaterial; }

        void                UpdateMaterial();

        DiString            GetTextureName() const { return mDesc->DemiureName; }

        void                SetTextureName(const DiString& val);

        DiByteMap*          GetDensityMap() const { return mDensityMap; }

        DiFoliageLayerDesc* GetDesc() const { return mDesc; }

    protected:

        void                CreateMaterial();

    protected:

        DiMaterialPtr       mMaterial;
        
        bool                mNeedUpdateMaterial;

        DiFoliageLayerDesc* mDesc;
        
        float               mWaveCount;

        DiByteMap*          mDensityMap;
    };
}