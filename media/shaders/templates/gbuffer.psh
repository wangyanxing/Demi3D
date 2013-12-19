
#include <common.hlsl>

struct VS_OUTPUT
{
	float4 Position 	: POSITION;
	float3 ViewPos	 	: TEXCOORD0;
	float3 ViewNormal 	: TEXCOORD1;
};

struct GBUFFER_OUTPUT
{
	float4 NormalDepth : COLOR0;
};

GBUFFER_OUTPUT ps_main( VS_OUTPUT In )
{			
	GBUFFER_OUTPUT Out;
	float clipDistance = g_farnearPlane.x - g_farnearPlane.y;
	Out.NormalDepth = float4(normalize(In.ViewNormal).xyz, (length(In.ViewPos) - g_farnearPlane.y) / clipDistance);
	
	return Out;
}

