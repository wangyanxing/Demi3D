
#include <common.hlsl>

#ifndef _SHOW_LAYER_0
#	define _SHOW_LAYER_0 1
#endif

#ifndef _SHOW_LAYER_1
#	define _SHOW_LAYER_1 1
#endif

#if _SHOW_LAYER_0
uniform sampler2D 	diffuseMap_0;
uniform sampler2D	normalMap_0;
#endif

#if _SHOW_LAYER_1
uniform sampler2D 	diffuseMap_1;
uniform sampler2D	normalMap_1;
#endif

struct VS_OUTPUT
{
	float4 Position 		: POSITION;
	float4 Color0			: COLOR;
	float2 Texcoord0		: TEXCOORD0;
	float3 Normal			: TEXCOORD1;
	float3 Tangent			: TEXCOORD2;
	float3 Binormal			: TEXCOORD3;
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

	half  specular0 = diffuse0.a;
	half  specular1 = diffuse1.a;
	half3 cSpecularmapColor = lerp(specular0, specular1, fAlpha);

	float3 cDiffuseColor = lerp(cDiffuseColor0.rgb, cDiffuseColor1.rgb, fAlpha) * In.Color0.rgb;

#if _SHOW_LAYER_0
	half3 cNormalmapColor0	= tex2D(normalMap_0, In.Texcoord0);
#else
	half3 cNormalmapColor0	= half3(0.5,0.5,1);
#endif

#if _SHOW_LAYER_1
	half3 cNormalmapColor1	= tex2D(normalMap_1, In.Texcoord0);
#else
	half3 cNormalmapColor1	= half3(0.5,0.5,1);
#endif

	half3 cNormalmapColor	= lerp(cNormalmapColor0, cNormalmapColor1, fAlpha);

	float3 vNormal = In.Normal;

	float Bump = 1.0f;
	float3 bump = Bump * (cNormalmapColor - float3(0.5,0.5,0.5));
    vNormal = vNormal + bump.x*In.Tangent + bump.y*In.Binormal;
    vNormal = normalize(vNormal);

	//float3 vDiffuse;
    //float3 vSpecular;
	//SunLightingSpec(vNormal,-g_eyeDirection,cSpecularmapColor,1,vDiffuse,vSpecular);

	//float3 vFinalColor = /*vSpecular+*/(cDiffuseColor*(vDiffuse+g_ambientColor));
    float3 vFinalColor = cDiffuseColor.rgb;

	Out.Color.rgb = vFinalColor;
	Out.Color.a = 1;
	
	return Out;
}