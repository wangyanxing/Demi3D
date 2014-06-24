
#include <common.hlsl>

struct VS_INPUT
{
	float4 Position   	: POSITION;

#if defined( SKINNED)
	float4 BlendWeights : BLENDWEIGHT;
	int4   BlendIndices : BLENDINDICES;
#endif	

#if defined( USE_MAP )
	float2 Texcoord0  	: TEXCOORD0;
#endif

#if defined( USE_LIGHTMAP )
	float2 Texcoord1  	: TEXCOORD1;
#endif

#if defined( USE_COLOR )
	half4  Color		: COLOR;
#endif
};

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

VS_OUTPUT vs_main( VS_INPUT In )
{
	float4 objPos = 0;
	GET_SPACE_POS

	VS_OUTPUT Out;

	Out.Position       	= mul(g_modelViewProjMatrix, objPos);

#if defined( USE_COLOR )
	Out.Color         	= In.Color;
#endif

#if defined( USE_MAP )
	Out.Texcoord0      	= In.Texcoord0;
#endif

#if defined( USE_LIGHTMAP )
	Out.Texcoord1 		= In.Texcoord1;
#endif
	
	return Out;
}