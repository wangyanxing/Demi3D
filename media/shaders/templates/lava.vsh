
#include <globals.hlsl>

uniform float2 uvScale;

struct VS_INPUT
{
	float4 Position   	: POSITION;
	float2 Texcoord0  	: TEXCOORD0;
};

struct VS_OUTPUT
{
	float4 Position 	: POSITION;
	float2 Texcoord0  	: TEXCOORD0;
};

VS_OUTPUT vs_main( VS_INPUT In )
{
	VS_OUTPUT Out;

	Out.Position = mul(g_modelViewProjMatrix,In.Position);
	Out.Texcoord0 = uvScale * In.Texcoord0;
	
	return Out;
}


