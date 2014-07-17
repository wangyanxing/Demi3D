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

#ifndef DiLight_h__
#define DiLight_h__

#include "TransformUnit.h"
#include "Shadows.h"
#include "Texture.h"

#define ENABLE_FSM 1

namespace Demi 
{
    /** Dynamic light types
        Currently the Skylight can only have one
        instance in the scene manager
        but actually it's enough
     */
    enum LightType
    {
        /// Typical directional light
        LIGHT_DIRECTIONAL = 0,
        
        /// Typical point light
        LIGHT_POINT,
        
        /// Hemisphere light
        LIGHT_SKY,
        
        LIGHT_SPOT,
        
        MAX_LIGHT_TYPES
    };
    
    /** A dynamic light source created by the scene manager
        lights should be attached to nodes in order to get
        the positions even full transformations
     */
    class DI_GFX_API DiLight : public DiTransformUnit
    {
    public:

        friend class DiRenderPipeline;
        friend class DiSceneManager;

        DiLight(LightType type);

        virtual                 ~DiLight(void);

    public:

        LightType               GetType(void) const { return mType; }

        const DiColor&          GetColor(void) const { return mColor; }

        /** Set the color of the light
            usually any type of light should have a color property
         */
        void                    SetColor(const DiColor &color) { mColor = color; }
        
        /** Get TransformObj type
         */
        DiString&               GetType(void);
        
        void                    InitForShadowCasting(DiSceneManager* sm, ShadowTextureConfig config);

        /** Set up shadow cameras
         */
        void                    SetupShadowCamera(DiSceneManager* sceneManager){}
        
        virtual DiVec3          GetDerivedDirection() const { return DiVec3::ZERO; }

        virtual DiVec3          GetDerivedPosition() const { return DiVec3::ZERO; }
        
        void                    SetShadowNearClipDistance(float nearClip) { mShadowNearClipDist = nearClip; }
        
		float                   GetShadowNearClipDistance() const { return mShadowNearClipDist; }
        
		float                   DeriveShadowNearClipDistance(const DiCamera* maincam) const;
        
		void                    SetShadowFarClipDistance(float farClip) { mShadowFarClipDist = farClip; }
        
		float                   GetShadowFarClipDistance() const { return mShadowFarClipDist; }
        
		virtual float           DeriveShadowFarClipDistance(const DiCamera* maincam) const;
        
        size_t                  GetNumShadowTextures() {return mShadowTextures.size(); }
        
        DiTexturePtr            GetShadowTexture(size_t id) {return mShadowTextures[id]; }
        
        DiCamera*               GetShadowCamera(size_t id) {return mShadowCameras[id]; }

    protected:

        void                    CreateShadowTextures(DiSceneManager* sm);

    protected:

        LightType               mType;

        DiColor                 mColor;

        float                   mShadowNearClipDist{ 0 };
        
		float                   mShadowFarClipDist{ 0 };

        DiVector<ShadowTextureConfig> mShadowConfig;
        
        DiVector<DiTexturePtr>  mShadowTextures;
        
        DiVector<DiCamera*>     mShadowCameras;
    };
}

#endif
