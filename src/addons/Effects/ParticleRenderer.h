
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

/****************************************************
  This module was originally from Particle Universe
  Repo: https://github.com/scrawl/particleuniverse
  License: MIT 
****************************************************/

#ifndef ParticleRenderer_h__
#define ParticleRenderer_h__

#include "FxPrerequisites.h"

namespace Demi
{
    class DEMI_FX_API DiParticleRenderer
    {
    public:
        bool autoRotate;

        static const uint8     DEFAULT_RENDER_QUEUE_GROUP;
        static const bool      DEFAULT_SORTED;
        static const uint8     DEFAULT_TEXTURECOORDS_ROWS;
        static const uint8     DEFAULT_TEXTURECOORDS_COLUMNS;
        static const bool      DEFAULT_USE_SOFT_PARTICLES;
        static const float     DEFAULT_SOFT_PARTICLES_CONTRAST_POWER;
        static const float     DEFAULT_SOFT_PARTICLES_SCALE;
        static const float     DEFAULT_SOFT_PARTICLES_DELTA;

    public:

        DiParticleRenderer(void);
        virtual ~DiParticleRenderer(void);

        const DiString&         GetRendererType(void) const;

        void                    SetRendererType(DiString rendererType);

        DiParticleElement*      GetParentElement(void) const;

        void                    SetParentElement(DiParticleElement* parentTechnique);

        bool                    IsRendererInitialised(void) const;

        void                    SetRendererInitialised(bool rendererInitialised);

        virtual void            NotifyStart(void);

        virtual void            NotifyStop(void);

        virtual void            NotifyRescaled(const DiVec3& scale);

        virtual void            SetVisible(bool visible = true) {mVisible = visible;};

        virtual void            Prepare(DiParticleElement*) = 0;

        virtual void            Unprepare(DiParticleElement*){/* No implementation */};

        inline virtual void     ProcessParticle(DiParticleElement* particleTechnique, 
                                    DiParticle* particle, 
                                    float timeElapsed, 
                                    bool firstParticle){}

        bool                    GetUseSoftParticles(void) const;

        void                    SetUseSoftParticles(bool useSoftParticles);

        float                   GetSoftParticlesContrastPower(void) const;

        float                   GetSoftParticlesScale(void) const;

        float                   GetSoftParticlesDelta(void) const;

        void                    SetSoftParticlesContrastPower(float softParticlesContrastPower);

        void                    SetSoftParticlesScale(float softParticlesScale);

        void                    SetSoftParticlesDelta(float softParticlesDelta);
        
        virtual void            Update(DiCamera*, DiParticlePool*){}
        
        virtual void            AddToBatchGroup(DiRenderBatchGroup*){}

        virtual void            SetMaterialName(const DiString& materialName) = 0;

        virtual void            NotifyCurrentCamera(DiCamera* cam) = 0;

        virtual void            NotifyAttached(DiNode* parent) = 0;

        virtual void            NotifyParticleQuota(size_t quota) = 0;

        virtual void            NotifyDefaultDimensions(float width, float height, float depth) = 0;

        virtual void            NotifyParticleResized(void) = 0;

        virtual void            NotifyParticleZRotated(void) {}

        virtual void            SetBatchGroupID(uint8 queueId);

        virtual uint8           GetBatchGroupID(void) const;

        virtual DiSortMode      GetSortMode(void) const = 0;

        const bool              IsSorted(void) const;

        void                    SetSorted(bool sorted);

        const uint8             GetTextureCoordsRows(void) const;

        void                    SetTextureCoordsRows(uint8 const textureCoordsRows);

        const uint8             GetTextureCoordsColumns(void) const;

        void                    SetTextureCoordsColumns(uint8 const textureCoordsColumns);

        size_t                  GetNumTextureCoords(void);

        virtual void            CopyTo(DiParticleRenderer* renderer);

        virtual void            CopyParentTo(DiParticleRenderer* renderer);

        void                    AddTextureCoords(const float u, const float v, const float width, const float height);

        void                    AddTextureCoords(const DiVector<DiFloatRect*>& rect);

        const DiVector<DiFloatRect*>& GetTextureCoords(void) const;

    protected:
        void                    CreateSoftMaterial(void);

        void                    StripNameFromSoftPrefix(DiString& name);

        DiParticleElement*      mParentTechnique;

        DiBillboardSet*         mBillboardSet;

        DiString                mRendererType;

        bool                    mCullIndividual;

        float                   mHeight;

        float                   mWidth;

        float                   mDepth;

        bool                    mSorted;

        uint8                   mQueueId;

        bool                    mRendererInitialised;

        DiVec3                  mRendererScale;

        DiVector<DiFloatRect*>  mUVList;

        uint8                   mTextureCoordsRows;

        uint8                   mTextureCoordsColumns;

        bool                    mTextureCoordsRowsAndColumnsSet;

        bool                    mTextureCoordsSet;

        // 软粒子相关，暂时不做

        bool                    mUseSoftParticles;

        float                   mSoftParticlesContrastPower;

        float                   mSoftParticlesScale;

        float                   mSoftParticlesDelta;

        bool                    mNotifiedDepthMap;

        bool                    mVisible;

        bool                    mDepthCheckEnabled;

        bool                    mDepthWriteEnabled;
    };
}

#endif // ParticleRenderer_h__
