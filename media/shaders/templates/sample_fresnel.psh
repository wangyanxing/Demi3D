
#include <common.hlsl>

uniform samplerCUBE cubeMap;

struct VS_OUTPUT
{
	float4 Position 	: POSITION;

	float3 Reflect      : TEXCOORD1;
	float3 Refract0 	: TEXCOORD2;
	float3 Refract1 	: TEXCOORD3;
	float3 Refract2 	: TEXCOORD4;
	float  ReflectFactor: TEXCOORD5;
};

PS_OUTPUT ps_main( VS_OUTPUT In )
{			
	PS_OUTPUT Out;

	float4 reflectedColor = texCUBE( cubeMap, float3( In.Reflect.xyz ) );
	float4 refractedColor = float4( 1.0,1.0,1.0,1.0 );
																							   
	refractedColor.r = texCUBE( cubeMap, float3( In.Refract0.xyz ) ).r;
	refractedColor.g = texCUBE( cubeMap, float3( In.Refract1.xyz ) ).g;
	refractedColor.b = texCUBE( cubeMap, float3( In.Refract2.xyz ) ).b;
																							   
	Out.Color = lerp( refractedColor, reflectedColor, clamp( In.ReflectFactor, 0.0, 1.0 ) );

	return Out;
}

