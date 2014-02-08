
#include <common.hlsl>

uniform const sampler2D image;
uniform const sampler2D imageBloom;

struct VS_OUTPUT
{
	float4 Position	 : POSITION;
	float2 Texcoord0 : TEXCOORD0;
};

PS_OUTPUT ps_main( VS_OUTPUT In )
{
	PS_OUTPUT Out;
	Out.Color = 0.0f;
	Out.Color += tex2D(image, In.Texcoord0);
	Out.Color += tex2D(imageBloom, In.Texcoord0 * 0.25);

	return Out;
}
