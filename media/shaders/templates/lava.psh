
#include <common.hlsl>

uniform float time;

uniform float fogDensity;
uniform float3 fogColor;

uniform sampler2D texture1;
uniform sampler2D texture2;

struct VS_OUTPUT
{
	float4 Position 	: POSITION1;
	float2 Texcoord0  	: TEXCOORD0;
};

PS_OUTPUT ps_main( VS_OUTPUT In )
{			
	PS_OUTPUT Out;
	
	float2 vUv = In.Texcoord0;
	float2 position = -1.0 + 2.0 * vUv;

	float4 noise = tex2D( texture1, vUv );
	float2 T1 = vUv + float2( 1.5, -1.5 ) * time  *0.02;
	float2 T2 = vUv + float2( -0.5, 2.0 ) * time * 0.01;

	T1.x += noise.x * 2.0;
	T1.y += noise.y * 2.0;
	T2.x -= noise.y * 0.2;
	T2.y += noise.z * 0.2;

	float p = tex2D( texture1, T1 * 2.0 ).a;

	float4 color = tex2D( texture2, T2 * 2.0 );
	float4 temp = color * ( float4( p, p, p, p ) * 2.0 ) + ( color * color - 0.1 );

	if( temp.r > 1.0 ){ temp.bg += clamp( temp.r - 2.0, 0.0, 100.0 ); }
	if( temp.g > 1.0 ){ temp.rb += temp.g - 1.0; }
	if( temp.b > 1.0 ){ temp.rg += temp.b - 1.0; }

	Out.Color = temp;

	float depth = In.Position.z / In.Position.w;
	const float LOG2 = 1.442695;
	float fogFactor = exp2( - fogDensity * fogDensity * depth * depth * LOG2 );
	fogFactor = 1.0 - clamp( fogFactor, 0.0, 1.0 );

	Out.Color = lerp( Out.Color, float4( fogColor, Out.Color.w ), fogFactor );
	
	return Out;
}