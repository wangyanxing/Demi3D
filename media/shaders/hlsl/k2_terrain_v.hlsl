
#include <common.hlsl>

uniform float4	v_WorldSizes;

struct VS_OUTPUT
{
	float4 Position 		: POSITION;
	float4 Color0			: COLOR;
	float2 Texcoord0		: TEXCOORD0;
	float3 Normal			: TEXCOORD1;
	float3 Tangent			: TEXCOORD2;
	float3 Binormal			: TEXCOORD3;
	float3 ViewDir	 	    : TEXCOORD4;
};

VS_OUTPUT vs_main(
	float height			: POSITION,
	int4 tc0				: TEXCOORD0,
	int4 tc1				: TEXCOORD1,
	half4 vertexColor		: COLOR
)
{
	VS_OUTPUT Out;

	int2 vTile = int2(tc0.w, tc1.w);
	
	float4 vPosition	= float4(vTile.x * v_WorldSizes.x, height, vTile.y * v_WorldSizes.x, 1.0);
	float3 vWorldPos	= mul(g_modelMatrix,vPosition);
	float2 vTexcoord0   = vTile * v_WorldSizes.y;
	
	float4 vNormal		= float4(tc0.xyz / 255.0 * 2.0 - 1.0,1);
	float4 vTangent 	= float4(tc1.xyz / 255.0 * 2.0 - 1.0,1);

	float3 vWVNormal	= vNormal.xyz;
	float3 vWVTangent	= vTangent.xyz;
	float3 vWVBinormal	= cross(vWVTangent, vWVNormal);

	Out.Normal			= mul(g_modelMatrix,vWVNormal);
	Out.Tangent   		= mul(g_modelMatrix,vWVTangent);
	Out.Binormal  		= mul(g_modelMatrix,vWVBinormal);

	Out.Position		= mul(g_modelViewProjMatrix, vPosition);
	Out.Color0			= vertexColor;
	Out.Texcoord0		= float4(vTexcoord0,0,0);
	Out.ViewDir         = g_eyePosition - vWorldPos;

	return Out;
}
