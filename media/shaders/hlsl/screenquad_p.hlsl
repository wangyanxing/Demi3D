
#include <common.hlsl>

uniform const sampler2D image;

struct VS_OUTPUT
{
	float4 Position 		: POSITION;
	float2 Texcoord 		: TEXCOORD0;
};

PS_OUTPUT ps_main( VS_OUTPUT In )
{
	PS_OUTPUT Out;
	Out.Color = tex2D(image,In.Texcoord);
	return Out;
}
