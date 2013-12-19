
#include <common.hlsl>

uniform const sampler2D image;

uniform float nIntensity;
uniform float sIntensity;
uniform float sCount;

struct VS_OUTPUT
{
	float4 Position 		: POSITION;
	float2 Texcoord 		: TEXCOORD0;
};

PS_OUTPUT ps_main( VS_OUTPUT In )
{
	PS_OUTPUT Out;
	float4 origin = tex2D(image, In.Texcoord);
	
	// make some noise
	float x = In.Texcoord.x * In.Texcoord.y * g_time *  1000.0;
	x = fmod( x, 13.0 ) * fmod( x, 123.0 );
	float dx = fmod( x, 0.01 );
	
	// apply noise
	float3 cResult = origin.rgb + origin.rgb * clamp( 0.1 + dx * 100.0, 0.0, 1.0 );
	
	float2 sc = float2( sin( In.Texcoord.y * sCount ), cos( In.Texcoord.y * sCount ) );
	
	cResult += origin.rgb * float3( sc.x, sc.y, sc.x ) * sIntensity;
	
	cResult = origin.rgb + clamp( nIntensity, 0.0,1.0 ) * ( cResult - origin.rgb );
	
	Out.Color =  float4( cResult, origin.a );
	
	return Out;
}
