
#include <common.hlsl>

struct VS_INPUT
{
	float4 Position   	: POSITION;
	
	float3 Normal		: NORMAL;

#if defined( SKINNED)
	float4 BlendWeights : BLENDWEIGHT;
	int4   BlendIndices : BLENDINDICES;
#endif

#if defined( USE_MAP )
	float2 Texcoord0  	: TEXCOORD0;
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
     
#if defined( USE_MAP ) || defined( USE_BUMPMAP ) || defined( USE_NORMALMAP )
	float2 Texcoord0  	: TEXCOORD0;
#endif

	float3 Normal       : TEXCOORD1;
	
	float3 ViewDir 		: TEXCOORD2;
	
	float3 PosWorld		: TEXCOORD3;

    float  Depth        : TEXCOORD4;
};

VS_OUTPUT vs_main( VS_INPUT In )
{
	float4 objPos = 0;
	float3 objNormal = 0;
	GET_SPACE_POS_NORMAL
	
	VS_OUTPUT Out;
	Out.Position       	= mul(g_modelViewProjMatrix,objPos);

#if defined( USE_COLOR )
	Out.Color         	= In.Color;
#endif

#if defined( USE_MAP )
	Out.Texcoord0      	= In.Texcoord0;
#endif

	// world space normal
    Out.Normal       	= mul(g_modelMatrix,objNormal).xyz;
	Out.PosWorld		= mul(g_modelMatrix,objPos);
	Out.ViewDir			= Out.PosWorld - g_eyePosition;

    float4 t = mul(g_modelViewMatrix, objPos);
    Out.Depth           = t.z / t.w;

	return Out;
}