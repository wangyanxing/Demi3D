
/********************************************************************
	date:		2012-4-19
	file: 		diffuse.psh
	creator:	Wang Yanxing
*********************************************************************/

#include <common.hlsl>

uniform const samplerCUBE environmentTexture;

struct VS_OUTPUT
{
	float3 Texcoord0		: TEXCOORD0;
};

PS_OUTPUT ps_main( VS_OUTPUT In )
{
	PS_OUTPUT Out;
	
	Out.Color = texCUBE(environmentTexture, In.Texcoord0);

	return Out;
}
