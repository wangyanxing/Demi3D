
#include <common.hlsl>

#ifdef USE_MAP
	uniform sampler2D map;
#endif

#ifdef USE_LIGHTMAP
	uniform sampler2D lightMap;
#endif

struct VS_OUTPUT
{
	float4 Position 	: POSITION;
	
#if defined( USE_COLOR )
	half4  Color		: COLOR;
#endif

#if defined( USE_MAP ) 
	float2 Texcoord0  	: TEXCOORD0;
#endif

#if defined( USE_LIGHTMAP )
	float2 Texcoord1	: TEXCOORD1;
#endif
};

PS_OUTPUT ps_main( VS_OUTPUT In )
{			
	PS_OUTPUT Out;
	Out.Color = float4( g_diffuseColor, g_opacity );
	
#ifdef USE_MAP
	float4 texelColor = tex2D( map, In.Texcoord0 );
	#ifdef GAMMA_INPUT
		texelColor.xyz *= texelColor.xyz;
	#endif
	Out.Color = Out.Color * texelColor;
#endif

#ifdef ALPHATEST
	if ( Out.Color.a < ALPHATEST ) discard;
#endif

#ifdef USE_LIGHTMAP
	Out.Color = Out.Color * tex2D( lightMap, In.Texcoord1 );
#endif
	
#ifdef USE_COLOR
    Out.Color = Out.Color * float4(In.Color.xyz, g_opacity);
#endif	

#ifdef GAMMA_OUTPUT
	Out.Color.xyz = sqrt( Out.Color.xyz );
#endif
	
	return Out;
}

