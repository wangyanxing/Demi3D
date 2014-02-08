
#include <common.hlsl>

uniform float4 vTexelSize;

struct VS_OUTPUT
{
	float4 Position  : POSITION;
	float2 Texcoord0 : TEXCOORD0;
};

struct VS_INPUT
{
	float2 Position  : POSITION;
	float4 Color0    : COLOR0;
	float2 Texcoord0 : TEXCOORD0;
};

VS_OUTPUT vs_main( VS_INPUT In )
{
	VS_OUTPUT Out;

	float4 v = float4(In.Position.x, In.Position.y, 0.0f, 1.0f);

	Out.Position  = mul(g_viewProjMatrix,v);
	Out.Texcoord0 = In.Texcoord0 * 0.25f;
	return Out;
}

