
#include <common.hlsl>

struct VS_INPUT
{
	float4 Position   	: POSITION;
	float2 Texcoord0  	: TEXCOORD0;
	half4  Color		: COLOR;
};

struct VS_OUTPUT
{
	float4 Position 	: POSITION;
	half4  Color		: COLOR;
	float2 Texcoord0  	: TEXCOORD0;
};

VS_OUTPUT vs_main( VS_INPUT In )
{
	float4 objPos = 0;
	GET_SPACE_POS

	VS_OUTPUT Out;

	Out.Position       	= mul(g_modelViewProjMatrix, objPos);

	Out.Color         	= In.Color;
	Out.Texcoord0      	= In.Texcoord0;
	return Out;
}