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

        DiShadowParams&         GetShadowParams(void) { return mShadowParams; }

        /** Set up shadow cameras
         */
        void                    SetupShadowCamera(DiSceneManager* sceneManager){}

        void                    UpdateSplitDist(DiCamera* camera);

        void                    UpdateFrustumPoints(ShadowFrustum &f, const DiVec3 &center, const DiVec3 &view_dir);

        void                    ApplyCropMatrix(DiCamera* shadowCam, ShadowFrustum& f);

    protected:

        void                    CreateShadowTextures();

    protected:

        LightType               mType;

        DiColor                 mColor;

        /// Shadow params
        DiShadowParams          mShadowParams;

        /// test
        ShadowFrustum           mShadowFrustums[MAX_CASCADE_SPLITS];
        
        bool                    mShadowCameraDirty;
        
        DiCamera*               mShadowCamera;

    public:

        DiTexturePtr            mShadowTextures[MAX_CASCADE_SPLITS];

        DiCamera*               mShadowCameras[MAX_CASCADE_SPLITS];

        float                   mCascadePartitionsFrustum[MAX_CASCADE_SPLITS];

    public:

        void                    GetShadowCamera(const DiCamera *cam, DiCamera *texCam, uint16 iteration);

        virtual void            GetShadowCameraForCascade(const DiCamera *cam, DiCamera *texCam,
                                    uint16 iteration, float nearSplit, float farSplit) {}

        void                    SetSplitPoints(const DiVector<float>& newSplitPoints);

        void                    CalculateSplitPoints(uint16 cascadeCount, float firstSplitDist, float farDist, float lambda);

        uint16                  mCascadeCount;
        
        DiVector<float>         mSplitPoints;

        float                   mSplitPadding;

        uint16                  mCurrentIteration;
    };
}

#endif
