
#include <common.hlsl>

struct VS_INPUT
{
	float4 Position   	: POSITION;
	float3 Normal		: NORMAL;
    float4 Tangent      : TANGENT;

#if defined( SKINNED)
	float4 BlendWeights : BLENDWEIGHT;
	int4   BlendIndices : BLENDINDICES;
#endif

	float2 Texcoord0  	: TEXCOORD0;
	half4  Color		: COLOR;
};

struct VS_OUTPUT
{
	float4 Position 	: POSITION;
	half4  Color		: COLOR;
	float4 Texcoord0  	: TEXCOORD0;
    //float4 Texcoord1    : TEXCOORD1;
	float3 Normal       : TEXCOORD2;
	float3 ViewDir 		: TEXCOORD3;
	float3 PosWorld		: TEXCOORD4;
    float3 Tangent      : TEXCOORD5;
    float3 Binormal     : TEXCOORD6;
};

uniform float fSpeed;

VS_OUTPUT vs_main( VS_INPUT In )
{
	float4 objPos = 0;
	float3 objNormal = 0;
    float3 objTangent = 0;
    GET_SPACE_POS_NORMAL_TANGENT
	
    float4 viewPos = mul(g_modelViewMatrix,objPos);

	VS_OUTPUT Out;
	Out.Position = mul(g_projMatrix,viewPos);

	Out.Color = In.Color;
    Out.Color.a *= Out.Color.a;

    Out.Texcoord0.xy = In.Texcoord0 + float2(fSpeed * g_time, fSpeed * g_time);
    Out.Texcoord0.zw = In.Texcoord0 * -1.5f + float2(fSpeed * g_time, fSpeed * g_time);

    float3 vNormal = objNormal.xyz;
    float3 vTangent = objTangent.xyz;
    vTangent = normalize(vTangent);

	// world space normal
    Out.Normal = mul(g_modelMatrix, float4(vNormal.xyz, 0.0)).xyz;
    Out.Tangent = mul(g_modelMatrix, float4(vTangent.xyz, 0.0)).xyz;
    Out.Binormal = cross(Out.Normal, Out.Tangent);

	Out.PosWorld = mul(g_modelMatrix,objPos).xyz;
    Out.ViewDir = g_eyePosition - Out.PosWorld;

	return Out;
}