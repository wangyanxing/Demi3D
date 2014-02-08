
#include <common.hlsl>

uniform const sampler2D image;

struct VS_OUTPUT
{
	float2 Texcoord0 : TEXCOORD0;
	float2 Texcoord1 : TEXCOORD1;
	float2 Texcoord2 : TEXCOORD2;
	float2 Texcoord3 : TEXCOORD3;
};

PS_OUTPUT ps_main( VS_OUTPUT In )
{
	PS_OUTPUT Out;
		
	Out.Color = 0.0f;
	
	Out.Color += tex2D(image, In.Texcoord0);
	Out.Color += tex2D(image, In.Texcoord1);
	Out.Color += tex2D(image, In.Texcoord2);
	Out.Color += tex2D(image, In.Texcoord3);
	
	Out.Color *= 0.25f;

	return Out;
}
