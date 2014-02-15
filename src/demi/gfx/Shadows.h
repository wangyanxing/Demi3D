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

#ifndef DiShadows_h__
#define DiShadows_h__

namespace Demi
{

#define MAX_CASCADE_SPLITS  4
#define SHADOW_MIN_QUANTIZE 0.1f
#define SHADOW_MIN_VIEW     1.0f
#define MAX_LIGHT_SPLITS    6

    /** General dynamic shadow parameters
    */
    struct DiShadowParams
    {
        DiShadowParams()
        : intensity(0.0f)
        , distance(0.0f)
        , resolution(1.0f)
        , nearFarRatio(0.002f)
        {
        }

        /// Shadow intensity
        float intensity;

        /// Fade start distance
        float distance;

        /// Shadow resolution
        float resolution;
        
        /// Shadow camera's near plane / far plane ratio
        float nearFarRatio;
    };

    /** Depth bias parameters for shadow mapping
     */
    struct DiShadowBiasParams
    {
        DiShadowBiasParams()
        : constantBias(0.0001f)
        , slopeScaledBias(0.5f)
        {
        }

        void Set(float constBias, float slopeBias)
        {
            constantBias = constBias;
            slopeScaledBias = slopeBias;
        }

        /** Clamp the values
         */
        void Validate()
        {
            constantBias = DiMath::Clamp(constantBias, -1.0f, 1.0f);
            slopeScaledBias = DiMath::Clamp(slopeScaledBias, -16.0f, 16.0f);
        }

        /// Constant bias.
        float constantBias;

        /// Slope scaled bias.
        float slopeScaledBias;
    };

    /** Focusing parameters for shadow mapping
     */
    struct DiShadowFocusParams
    {
        DiShadowFocusParams()
        :focus(true), nonUniform(true),
        autoSize(true), quantize(0.5f), minView(3.0f)
        {
        }

        void    Set(bool focus, bool nonUniform,
                    bool autoSize, float quantize, float minView)
        {
            this->focus      = focus;
            this->nonUniform = nonUniform;
            this->autoSize   = autoSize;
            this->quantize   = quantize;
            this->minView    = minView;
        }

        /** Clamp the values
        */
        void    Validate()
        {
            quantize = DiMath::Max(quantize, SHADOW_MIN_QUANTIZE);
            minView  = DiMath::Max(minView, SHADOW_MIN_VIEW);
        }

        /// Focus flag.
        bool    focus;
        
        /// Non-uniform focusing flag.
        bool    nonUniform;
        
        /// Auto-size (reduce resolution when far away) flag.
        bool    autoSize;
        
        /// Focus quantization.
        float   quantize;

        /// Minimum view size.
        float   minView;
    };

    /** CSM parameters
    */
    struct DI_GFX_API DiCascadeParams
    {
        /** Simply do nothing
        */
        DiCascadeParams()
        {
        }

        /** Set the splits and fade params
        */
        void  Set(float split1, float split2, float split3, float split4,
                  float fadeStart, float biasAutoAdjust = 1.0f)
        {
            splits[0] = split1;
            splits[1] = split2;
            splits[2] = split3;
            splits[3] = split4;

            this->fadeStart = fadeStart;
            this->biasAutoAdjust = biasAutoAdjust;
        }

        /** Validate parameters.
        */
        void  Validate(void)
        {
            for (unsigned i = 0; i < MAX_CASCADE_SPLITS; ++i)
                splits[i] = DiMath::Max(splits[i], 0.0f);
            fadeStart = DiMath::Clamp(fadeStart, std::numeric_limits<float>::epsilon(), 1.0f);
        }

        /** Return shadow maximum range.
        */
        float GetShadowRange(void) const
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
}

#endif