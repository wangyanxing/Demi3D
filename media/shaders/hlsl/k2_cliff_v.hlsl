
#include <common.hlsl>

struct VS_INPUT
{
	float4 Position   	: POSITION;

#if defined( SKINNED)
	float4 BlendWeights : BLENDWEIGHT;
	int4   BlendIndices : BLENDINDICES;
#endif
	
	float3 Normal		: NORMAL;

#if defined( USE_NORMALMAP )
    float4 Tangent      : TANGENT;
#endif

#if defined( USE_MAP ) || defined( USE_NORMALMAP ) || defined( USE_SPECULARMAP )
	float2 Texcoord0  	: TEXCOORD0;
#endif

#if defined( USE_COLOR )
	half4  Color		: COLOR;
#endif
};

static float fWorldTextureProjection = 0.002;

struct VS_OUTPUT
{
    float4 Position 	: POSITION;

    //half4  Color		: COLOR;

    float4 Texcoord0  	: TEXCOORD0;
    float3 Normal       : TEXCOORD1;
    float3 ViewDir	 	: TEXCOORD2;
    float3 PosWorld 	: TEXCOORD3;

#if defined( USE_NORMALMAP )
    float3 Tangent		: TEXCOORD4;
    float3 Binormal		: TEXCOORD5;
    float3 TerrainTangent  : TEXCOORD6;
    float3 TerrainBinormal : TEXCOORD7;
#endif
};

VS_OUTPUT vs_main( VS_INPUT In )
{
	float4 objPos = 0;
	float3 objNormal = 0;
#if defined( USE_NORMALMAP )
    float3 objTangent = 0;
    GET_SPACE_POS_NORMAL_TANGENT
#else
    GET_SPACE_POS_NORMAL
#endif
	
	VS_OUTPUT Out;

	Out.Position  = mul(g_modelViewProjMatrix,objPos);
	Out.Texcoord0.xy = In.Texcoord0.xy;
    Out.Texcoord0.zw = mul(g_modelMatrix, objPos).xz * fWorldTextureProjection;

    // world space normal
	float4x4 model;
    Out.Normal    = mul(g_modelMatrix, objNormal).xyz;
	Out.PosWorld  = mul(g_modelMatrix, objPos);
    Out.ViewDir   = g_eyePosition - Out.PosWorld;

#if defined( USE_NORMALMAP )
    Out.Tangent = mul(g_modelMatrix, float4(objTangent.xyz, 0.0)).xyz;
    Out.Binormal = cross(Out.Normal, Out.Tangent);
    float3 vTerrainTan = float3(1.0f, 0.0f, 0.0f);
    vTerrainTan -= Out.Normal * dot(Out.Normal, vTerrainTan);
    Out.TerrainTangent = normalize(vTerrainTan);
    Out.TerrainBinormal = cross(Out.TerrainTangent, Out.Normal);
#endif
	
	return Out;
}