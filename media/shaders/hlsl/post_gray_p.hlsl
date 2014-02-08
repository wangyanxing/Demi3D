
#include <common.hlsl>

uniform const sampler2D image;
uniform const float grayRate;

struct VS_OUTPUT
{
	float4 Position 		: POSITION;
	float2 Texcoord 		: TEXCOORD0;
};

PS_OUTPUT ps_main( VS_OUTPUT In )
{
	PS_OUTPUT Out;
	float3 origin = tex2D(image, In.Texcoord).rgb;
	float3 greyscale = dot(origin, float3(0.3, 0.59, 0.11));
	float3 final = lerp(origin,greyscale,grayRate);

	Out.Color = float4(final,1);

	return Out;
}
