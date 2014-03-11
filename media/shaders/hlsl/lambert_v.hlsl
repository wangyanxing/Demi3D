
#include <common.hlsl>

struct VS_INPUT
{
	float4 Position   	: POSITION;
	
	float3 Normal		: NORMAL;

#if defined( USE_NORMALMAP )
    float4 Tangent      : TANGENT;
#endif

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
     
#if defined( USE_MAP ) || defined( USE_NORMALMAP )
	float2 Texcoord0  	: TEXCOORD0;
#endif

	float3 Normal       : TEXCOORD1;
	
	float3 ViewDir 		: TEXCOORD2;
	
	float3 PosWorld		: TEXCOORD3;

#if USE_SHADOW
    float  Depth : TEXCOORD4;
    float4 ShadowLightspacePos : TEXCOORD5;
#endif

#if defined( USE_NORMALMAP )
    float3 Tangent      : TEXCOORD6;
    float3 Binormal     : TEXCOORD7;
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
	
    float4 viewPos = mul(g_modelViewMatrix,objPos);

	VS_OUTPUT Out;
	Out.Position = mul(g_projMatrix,viewPos);

#if defined( USE_COLOR )
	Out.Color = In.Color;
#endif

#if defined( USE_MAP )
	Out.Texcoord0 = In.Texcoord0;
#endif

	// world space normal
    Out.Normal = mul(g_modelMatrix, float4(objNormal.xyz, 0.0)).xyz;
#if defined( USE_NORMALMAP )
    Out.Tangent = mul(g_modelMatrix, float4(objTangent.xyz, 0.0)).xyz;
    Out.Binormal = cross(Out.Normal, Out.Tangent) ;
#endif
	Out.PosWorld = mul(g_modelMatrix,objPos).xyz;
	Out.ViewDir  = Out.PosWorld - g_eyePosition;

#if USE_SHADOW
    Out.Depth = dot(Out.Position.zw, Out.Position.zw);
    Out.ShadowLightspacePos = mul(g_firstCascadeTexMat, float4(Out.PosWorld.xyz, 1.0));
#endif

	return Out;
}