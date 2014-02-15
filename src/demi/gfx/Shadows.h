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
        DiShadowParams();

        void SetBias(float constBias, float slopeBias);

        void SetFocus(bool focus, bool nonUniform,
                      bool autoSize, float quantize, float minView);

        void SetCascade(float split1, float split2, float split3, float split4,
                      float fadeStart, float biasAutoAdjust = 1.0f);

        void Validate();

        /** Return shadow maximum range.
        */
        float GetShadowRange(void) const;

        /// Shadow intensity
        float intensity;

        /// Fade start distance
        float distance;

        /// Shadow resolution
        float resolution;
        
        /// Shadow camera's near plane / far plane ratio
        float nearFarRatio;

        /// Constant bias.
        float constantBias;

        /// Slope scaled bias.
        float slopeScaledBias;

        /// Focus flag.
        bool focus;

        /// Non-uniform focusing flag.
        bool nonUniform;

        /// Auto-size (reduce resolution when far away) flag.
        bool autoSize;

        /// Focus quantization.
        float quantize;

        /// Minimum view size.
        float minView;

        /// Far clip values of the splits.
        float splits[4];

        /// The point relative to the total shadow range where shadow fade begins (0.0 - 1.0)
        float fadeStart;

        /// Automatic depth bias adjustment strength.
        float biasAutoAdjust;

        /// Number of splits
        int   numSplits;

        /// Shadow cameras
        DiCamera* shadowCameras[MAX_LIGHT_SPLITS];

        /// Shadow caster start indices.
        uint16 shadowCasterBegin[MAX_LIGHT_SPLITS];

        /// Shadow caster end indices.
        uint16 shadowCasterEnd[MAX_LIGHT_SPLITS];

        /// Combined bounding box of shadow casters in light view or projection space.
        DiAABB shadowCasterBox[MAX_LIGHT_SPLITS];

        /// Shadow camera near splits (directional lights only.)
        float shadowNearSplits[MAX_LIGHT_SPLITS];
        
        /// Shadow camera far splits (directional lights only.)
        float shadowFarSplits[MAX_LIGHT_SPLITS];
    };

    /** Shadow camera pool
     */
    struct CameraPool
    {
        uint32              currentID;
        
        DiVector<DiCamera*> cameras;

        DiSceneManager*     sceneManager;

        CameraPool(DiSceneManager* sm);

        ~CameraPool();

        /** Get a camera from the pool
         */
        DiCamera* GetCamera();
        
        void Reset();
    };
}

#endif