
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
	Out.Color = float4( g_diffuseColor, g_opacity );

	float4 texelColor = tex2D( map, In.Texcoord0 );
	Out.Color = Out.Color * texelColor;

#ifdef ALPHATEST
	if ( Out.Color.a < ALPHATEST ) discard;
#endif

    Out.Color = Out.Color * float4(In.Color.xyz, In.Color.a);

	return Out;
}

