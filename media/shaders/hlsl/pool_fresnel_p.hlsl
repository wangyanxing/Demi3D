
#include <common.hlsl>

uniform sampler2D 	noiseMap;
uniform sampler2D 	reflectMap;
uniform sampler2D 	refractMap;

uniform float4		tintColour;
uniform float 		noiseScale;
uniform float 		fresnelBias;
uniform float 		fresnelScale;
uniform float 		fresnelPower;

struct VS_OUTPUT
{
	float4 Position 		: POSITION;
	float2 NoiseCoord		: TEXCOORD0;
	float4 ProjectionCoord	: TEXCOORD1;
	float3 EyeDir			: TEXCOORD2;
	float3 Normal			: TEXCOORD3;
};

PS_OUTPUT ps_main( VS_OUTPUT In )
{
	PS_OUTPUT Out;

	float2 final = In.ProjectionCoord.xy / In.ProjectionCoord.w;
	float3 noiseNormal = (tex2D(noiseMap, (In.NoiseCoord.xy / 5)).rgb - 0.5).rbg * noiseScale;
	final += noiseNormal.xz;

	// eye * normal
	float fresnel = fresnelBias + fresnelScale * 
		pow(1 + dot(normalize(In.Normal), normalize(In.EyeDir)), fresnelPower);

	float4 reflectionColour = tex2D(reflectMap, final);
	float4 refractionColour = tex2D(refractMap, final) + tintColour;

	Out.Color = lerp(refractionColour, reflectionColour, fresnel);

	return Out;
}
