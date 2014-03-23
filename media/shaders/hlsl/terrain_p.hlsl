
#include <common.hlsl>

#ifndef _SHOW_LAYER_0
#	define _SHOW_LAYER_0 1
#endif

#ifndef _SHOW_LAYER_1
#	define _SHOW_LAYER_1 1
#endif

#if _SHOW_LAYER_0
uniform sampler2D diffuseMap_0;
uniform sampler2D normalMap_0;
uniform sampler2D specularMap_0;
#endif

#if _SHOW_LAYER_1
uniform sampler2D diffuseMap_1;
uniform sampler2D normalMap_1;
uniform sampler2D specularMap_1;
#endif

struct VS_OUTPUT
{
    float4 Position : POSITION;
    float4 Color0   : COLOR;
    float2 Texcoord0: TEXCOORD0;
    float3 Normal   : TEXCOORD1;
    float3 Tangent  : TEXCOORD2;
    float3 Binormal	: TEXCOORD3;
    float3 ViewDir  : TEXCOORD4;
};

PS_OUTPUT ps_main( VS_OUTPUT In )
{
	PS_OUTPUT Out;

	float fAlpha = In.Color0.a;

#if _SHOW_LAYER_0
	half4 diffuse0 = tex2D(diffuseMap_0,In.Texcoord0.xy);
#else
	half4 diffuse0 = half4(1,1,1,1);
#endif

#if _SHOW_LAYER_1
	half4 diffuse1 = tex2D(diffuseMap_1,In.Texcoord0.xy);
#else
	half4 diffuse1 = diffuse0;
#endif
	half3 cDiffuseColor0 = diffuse0.rgb;
	half3 cDiffuseColor1 = diffuse1.rgb;

	float3 cDiffuseColor = lerp(cDiffuseColor0.rgb, cDiffuseColor1.rgb, fAlpha) * In.Color0.rgb;

#if _SHOW_LAYER_0
	half4 cNormalmapColor0;
    cNormalmapColor0.rgb = tex2D(normalMap_0, In.Texcoord0).agb;
	cNormalmapColor0.a = tex2D(specularMap_0, In.Texcoord0).g;
#else
	half4 cNormalmapColor0 = half3(0.5,0.5,1,1.0);
#endif

#if _SHOW_LAYER_1
	half4 cNormalmapColor1;
    cNormalmapColor1.rgb = tex2D(normalMap_1, In.Texcoord0).agb;
	cNormalmapColor1.a = tex2D(specularMap_1, In.Texcoord0).g;
#else
	half4 cNormalmapColor1 = half3(0.5,0.5,1,1.0);
#endif

	half4 cNormalmapColor = lerp(cNormalmapColor0, cNormalmapColor1, fAlpha);
	half3 texNormal = half3(cNormalmapColor.rgb * 2.0f - 1.0f);
    float3x3 rot = float3x3(In.Tangent, In.Binormal, In.Normal);
	half3 vNormal = normalize(mul(texNormal, rot));
	
	half3 vDiffuse = g_globalAmbient.rgb;
	half3 vSpecular = half3(0.0f, 0.0f, 0.0f);
	
	for (int i = 0; i < g_numDirLights; ++i)
	{
		float NdotL = saturate(dot(vNormal, -g_dirLightsDir[i]));
		half3 halfVec = normalize(-g_dirLightsDir[i] + normalize(In.ViewDir));
		float dotHalf = saturate(dot(vNormal, halfVec));
		float specWeight = saturate(pow(dotHalf, g_shininess));

		vDiffuse += g_dirLightsColor[i] * NdotL;
		vSpecular += g_dirLightsColor[i] * specWeight * cNormalmapColor.a;
	}

    half3 vFinalColor = cDiffuseColor.rgb * vDiffuse + vSpecular;

	Out.Color.rgb = vFinalColor;
	Out.Color.a = 1;
	
	return Out;
}