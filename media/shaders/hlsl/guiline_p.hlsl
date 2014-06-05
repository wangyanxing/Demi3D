
#include <common.hlsl>

uniform sampler2D map;

struct VS_OUTPUT
{
	float4 Position 	: POSITION;
	half4  Color		: COLOR;
	float2 Texcoord0  	: TEXCOORD0;
};

PS_OUTPUT ps_main( VS_OUTPUT In )
{			
	PS_OUTPUT Out;
    Out.Color = In.Color;
	return Out;
}