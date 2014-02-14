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
    
#define MAX_CASCADE_SPLITS 4
    
    /** CSM parameters
     */
    struct DI_GFX_API CascadeParams
    {
        /** Simply do nothing
         */
        CascadeParams()
        {
        }
        
        /** Set the splits and fade params
         */
        void Set(float split1, float split2, float split3, float split4,
                          float fadeStart, float biasAutoAdjust = 1.0f) :
        
            fadeStart(fadeStart),
            biasAutoAdjust(biasAutoAdjust)
        {
            splits[0] = split1;
            splits[1] = split2;
            splits[2] = split3;
            splits[3] = split4;
        }
        
        /** Validate parameters.
         */
        void Validate()
        {
            for (unsigned i = 0; i < MAX_CASCADE_SPLITS; ++i)
                splits_[i] = DiMax(splits[i], 0.0f);
            fadeStart_ = DiMath::Clamp(fadeStart, std::numeric_limits<float>::epsilon(), 1.0f);
        }
        
        /** Return shadow maximum range.
         */
        float GetShadowRange() const
        {
            float ret = 0.0f;
            for (unsigned i = 0; i < MAX_CASCADE_SPLITS; ++i)
                ret = DiMath::Max(ret, splits[i]);
            
            return ret;
        }
        
        /// Far clip values of the splits.
        float splits[4];
        
        /// The point relative to the total shadow range where shadow fade begins (0.0 - 1.0)
        float fadeStart;
        
        /// Automatic depth bias adjustment strength.
        float biasAutoAdjust;
    };
    
    /** A dynamic light source created by the scene manager
        lights should be attached to nodes in order to get
        the positions even full transformations
     */
    class DI_GFX_API DiLight : public DiTransformUnit
    {
    protected:

        friend class DiRenderPipeline;
        friend class DiSceneManager;

        DiLight(LightType type, DiSceneManager* mgr);

        virtual         ~DiLight(void);

    public:

        LightType       GetType() const { return mType; }

        const DiColor&  GetColor(void) const{ return mColor; }

        /** Set the color of the light
            usually any type of light should have a color property
         */
        void            SetColor(const DiColor &color) { mColor = color; }
        
        DiString&       GetType();
        
        CascadeParams&  GetCSMParams() { return mCascadeParams; }

    protected:

        LightType       mType;

        DiColor         mColor;

        DiSceneManager* mCreator;

        /// The relative camera regarding the shadow processing
        DiCamera*       mShadowCamera;
        
        /// for cascade shadow mapping
        CascadeParams   mCascadeParams;
    };
}

#endif
