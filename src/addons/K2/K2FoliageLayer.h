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

#ifndef DiFoliageLayer_h__
#define DiFoliageLayer_h__

#include "K2Prerequisites.h"
#include "Material.h"
#include "K2TerrainDesc.h"
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

    class DEMI_K2_API DiFoliageLayer
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

        void                SetRenderTechnique(GrassTechnique style, bool blendBase = false);

        void                SetAnimationEnabled(bool enabled);

        void                SetLightingEnabled(bool enabled);

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

#endif
