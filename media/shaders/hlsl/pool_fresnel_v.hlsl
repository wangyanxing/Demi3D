
#include <common.hlsl>

uniform float scale;       
uniform float scroll;

struct VS_OUTPUT
{
	float4 Position 		: POSITION;
	float2 NoiseCoord		: TEXCOORD0;
	float4 ProjectionCoord	: TEXCOORD1;
	float3 EyeDir			: TEXCOORD2;
	float3 Normal			: TEXCOORD3;
};

VS_OUTPUT vs_main(
	float4 pos			: POSITION,
	float4 normal		: NORMAL,
	float2 tex			: TEXCOORD0
	)
{
	VS_OUTPUT OUT;
	OUT.Position = mul(g_modelViewProjMatrix, pos);
	
	float4x4 scalemat = float4x4(0.5,   0,   0, 0.5, 
	                               0,-0.5,   0, 0.5,
								   0,   0, 0.5, 0.5,
								   0,   0,   0,   1);
	OUT.ProjectionCoord = mul(scalemat, OUT.Position);

	OUT.NoiseCoord.xy = (tex + (g_time * scroll)) * scale;
	
	float4 posWorld  = mul(g_modelMatrix, pos);

	OUT.EyeDir = (posWorld-g_eyePosition).xyz; 
	OUT.Normal = mul(g_modelMatrix, normal).xyz; 

	return OUT;
}