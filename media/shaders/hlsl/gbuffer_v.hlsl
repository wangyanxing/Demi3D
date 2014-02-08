
#include <globals.hlsl>

struct VS_INPUT
{
	float4 Position   	: POSITION;
	float3 Normal   	: NORMAL;
};

struct VS_OUTPUT
{
	float4 Position 	: POSITION;
	float3 ViewPos	 	: TEXCOORD0;
	float3 ViewNormal 	: TEXCOORD1;
};

VS_OUTPUT vs_main( VS_INPUT In )
{
	VS_OUTPUT Out;

	Out.Position       	= mul(g_modelViewProjMatrix,In.Position);
	Out.ViewPos 		= mul(g_modelViewMatrix, In.Position).xyz;       
	Out.ViewNormal 		= mul(g_modelViewMatrix, float4(In.Normal,0)).xyz;
	
	return Out;
}