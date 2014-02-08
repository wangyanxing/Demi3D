
#include <common.hlsl>

uniform const sampler2D image;

float3	vLinearBright;

struct VS_OUTPUT
{
	float2 Texcoord0 : TEXCOORD0;
};

PS_OUTPUT ps_main( VS_OUTPUT In )
{
	PS_OUTPUT Out;
	
	float4 vColor = tex2D(image, In.Texcoord0);
	
	Out.Color.rgb = vColor.rgb;
	
	float fBrightness = max(Out.Color.r, max(Out.Color.g, Out.Color.b)) + dot(Out.Color.rgb, float3(1.0f, 1.0f, 1.0f));
	float fScale = saturate(fBrightness * vLinearBright.x + vLinearBright.y) * vLinearBright.z;

	Out.Color.rgb *= fScale;
	Out.Color.a = vColor.a;

	return Out;
}
