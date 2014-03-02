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

half getShadowFactor(
    sampler2D shadowMapUnit,
    float4 lightSpacePos,
    float shadowmapSize,
    float inverseShadowmapSize,
    float fixedDepthBias,
    float gradientScaleBias,
    half shadowLightDotLN)
{
    // point on shadowmap
    float depthAdjust = fixedDepthBias + (1.0f - abs(shadowLightDotLN)) * gradientScaleBias;
    lightSpacePos.z -= depthAdjust; // lightSpacePos.z contains lightspace position of current object

    // Sample each of them checking whether the pixel under test is shadowed or not
    float lightMask = (lightSpacePos.z < tex2D(shadowMapUnit, lightSpacePos.xy,0,0).r);

    // Hack to prevent these getting optimized out, thereby preventing OGRE errors
    lightMask += 0.001 * (0.001*shadowmapSize + inverseShadowmapSize);

    // Get the average
    return lightMask;
}

#ifdef DEBUG_CSM
half3 getCsmShadowFactor
#else
half getCsmShadowFactor
#endif
(float4 lightSpacePos0, half shadowLightDotLN)
{
    half factor = 1;

#ifdef DEBUG_CSM
    half3 cascadeColor = float3(1, 1, 1);
#endif

    if (lightSpacePos0.x > 0.01 && lightSpacePos0.y > 0.01 && lightSpacePos0.x < 0.99 && lightSpacePos0.y < 0.99)
    {
        factor = getShadowFactor(g_shadowTexture0, lightSpacePos0, g_shadowMapParams.z, g_shadowMapParams.w,
            g_fixedDepthBias.x, g_gradientScaleBias.x, shadowLightDotLN);

#ifdef DEBUG_CSM
        cascadeColor = float3(1, 0, 0);
#endif
    }
    else
    {
        float4 lightSpacePos1;
        lightSpacePos1.xyz = lightSpacePos0.xyz + g_texMatrixScaleBias[0].xyz;
        lightSpacePos1.xy *= g_texMatrixScaleBias[0].w;

        if (lightSpacePos1.x > 0.01 && lightSpacePos1.y > 0.01 && lightSpacePos1.x < 0.99 && lightSpacePos1.y < 0.99)
        {
            factor = getShadowFactor(g_shadowTexture1, lightSpacePos1, g_shadowMapParams.z, g_shadowMapParams.w,
                g_fixedDepthBias.y, g_gradientScaleBias.y, shadowLightDotLN);

#ifdef DEBUG_CSM
            cascadeColor = float3(0, 1, 0);
#endif
        }
        else
        {
            float4 lightSpacePos2;
            lightSpacePos2.xyz = lightSpacePos0.xyz + g_texMatrixScaleBias[1].xyz;
            lightSpacePos2.xy *= g_texMatrixScaleBias[1].w;

            if (lightSpacePos2.x > 0.01 && lightSpacePos2.y > 0.01 && lightSpacePos2.x < 0.99 && lightSpacePos2.y < 0.99)
            {
                factor = getShadowFactor(g_shadowTexture2, lightSpacePos2, g_shadowMapParams.z, g_shadowMapParams.w,
                    g_fixedDepthBias.z, g_gradientScaleBias.z, shadowLightDotLN);

#ifdef DEBUG_CSM
                cascadeColor = float3(0, 0, 1);
#endif
            }
            else
            {
                float4 lightSpacePos3;
                lightSpacePos3.xyz = lightSpacePos0.xyz + g_texMatrixScaleBias[2].xyz;
                lightSpacePos3.xy *= g_texMatrixScaleBias[2].w;

                factor = getShadowFactor(g_shadowTexture3, lightSpacePos3, g_shadowMapParams.z, g_shadowMapParams.w,
                    g_fixedDepthBias.w, g_gradientScaleBias.w, shadowLightDotLN);

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
