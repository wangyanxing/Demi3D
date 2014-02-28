/*  Copyright 2010-2012 Matthew Paul Reid
Copyright 2010-2012 Matthew Paul Reid

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and 
associated documentation files (the "Software"), to deal in the Software without restriction, 
including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, 
and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do 
so, subject to the following conditions:

The above copyright notice and this permission notice shall be 
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT 
LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. 
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE 
OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

uniform float4 texMatrixScaleBias1;
uniform float4 texMatrixScaleBias2;
uniform float4 texMatrixScaleBias3;
uniform float4 fixedDepthBias;
uniform float4 gradientScaleBias;
uniform float4 shadowMapSize;
uniform float4 invShadowMapSize;

float4 tex2D_inBranch(uniform sampler2D sampler, float2 texCoords)
{
#ifdef TEX2D_FORCE_ZERO_GRAD_IN_BRANCH
    return tex2D(sampler, texCoords, 0, 0);
#else
    return tex2D(sampler, texCoords);
#endif
}

#ifdef DEBUG_CSM
half3 getCsmShadowFactor
#else
half getCsmShadowFactor
#endif
(
uniform sampler2D shadowTexture0,
uniform sampler2D shadowTexture1,
uniform sampler2D shadowTexture2,
uniform sampler2D shadowTexture3,
float4 lightSpacePos0,
half shadowLightDotLN
)
{
    half factor = 1;

#ifdef DEBUG_CSM
    half3 cascadeColor = float3(1, 1, 1);
#endif

    if (lightSpacePos0.x > 0.01 && lightSpacePos0.y > 0.01 && lightSpacePos0.x < 0.99 && lightSpacePos0.y < 0.99)
    {
        factor = getShadowFactor(shadowTexture0, lightSpacePos0, shadowMapSize.x, invShadowMapSize.x,
            fixedDepthBias.x, gradientScaleBias.x, shadowLightDotLN);

#ifdef DEBUG_CSM
        cascadeColor = float3(1, 0, 0);
#endif
    }
    else
    {
        float4 lightSpacePos1;
        lightSpacePos1.xyz = lightSpacePos0.xyz + texMatrixScaleBias1.xyz;
        lightSpacePos1.xy *= texMatrixScaleBias1.w;

        if (lightSpacePos1.x > 0.01 && lightSpacePos1.y > 0.01 && lightSpacePos1.x < 0.99 && lightSpacePos1.y < 0.99)
        {
            factor = getShadowFactor(shadowTexture1, lightSpacePos1, shadowMapSize.y, invShadowMapSize.y,
                fixedDepthBias.y, gradientScaleBias.y, shadowLightDotLN);

#ifdef DEBUG_CSM
            cascadeColor = float3(0, 1, 0);
#endif
        }
        else
        {
            float4 lightSpacePos2;
            lightSpacePos2.xyz = lightSpacePos0.xyz + texMatrixScaleBias2.xyz;
            lightSpacePos2.xy *= texMatrixScaleBias2.w;

            if (lightSpacePos2.x > 0.01 && lightSpacePos2.y > 0.01 && lightSpacePos2.x < 0.99 && lightSpacePos2.y < 0.99)
            {
                factor = getShadowFactor(shadowTexture2, lightSpacePos2, shadowMapSize.z, invShadowMapSize.z,
                    fixedDepthBias.z, gradientScaleBias.z, shadowLightDotLN);

#ifdef DEBUG_CSM
                cascadeColor = float3(0, 0, 1);
#endif
            }
            else
            {
                float4 lightSpacePos3;
                lightSpacePos3.xyz = lightSpacePos0.xyz + texMatrixScaleBias3.xyz;
                lightSpacePos3.xy *= texMatrixScaleBias3.w;

                factor = getShadowFactor(shadowTexture3, lightSpacePos3, shadowMapSize.w, invShadowMapSize.w,
                    fixedDepthBias.w, gradientScaleBias.w, shadowLightDotLN);

                // Fade out to edges
                half weight = saturate((max(abs(lightSpacePos3.x - 0.5), abs(lightSpacePos3.y - 0.5)) - 0.375) * 8);
                factor = lerp(factor, 1, weight);

#ifdef DEBUG_CSM
                cascadeColor = float3(1, 1, weight);
#endif
            }
        }
    }

#ifdef DEBUG_CSM
    return factor * cascadeColor;
#else
    return factor;
#endif
}

half getShadowFactor(
    uniform sampler2D shadowMapUnit,
    float4 lightSpacePos,
    uniform float shadowmapSize,
    uniform float inverseShadowmapSize,
    uniform float fixedDepthBias,
    uniform float gradientScaleBias,
    half shadowLightDotLN)
{
    // point on shadowmap
    float depthAdjust = fixedDepthBias + (1.0f - abs(shadowLightDotLN)) * gradientScaleBias;
    lightSpacePos.z -= depthAdjust; // lightSpacePos.z contains lightspace position of current object

    // Sample each of them checking whether the pixel under test is shadowed or not
    float lightMask = (lightSpacePos.z < tex2D_inBranch(shadowMapUnit, lightSpacePos.xy).r);

    // Hack to prevent these getting optimized out, thereby preventing OGRE errors
    lightMask += 0.001 * (0.001*shadowmapSize + inverseShadowmapSize);

    // Get the average
    return lightMask;
}
