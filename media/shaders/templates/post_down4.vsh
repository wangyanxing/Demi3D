
/********************************************************************
	日期:		2012-5-14
	文件名: 		post_down4.psh
	创建者:		王延兴
	
	描述:		缩放到四分之一VP大小
*********************************************************************/

#include <common.hlsl>

struct VS_OUTPUT
{
	float4 Position  : POSITION;
	float2 Texcoord0 : TEXCOORD0;
	float2 Texcoord1 : TEXCOORD1;
	float2 Texcoord2 : TEXCOORD2;
	float2 Texcoord3 : TEXCOORD3;
};

struct VS_INPUT
{
	float2 Position  : POSITION;
	float4 Color0    : COLOR0;
	float2 Texcoord0 : TEXCOORD0;
};

VS_OUTPUT vs_main( VS_INPUT In )
{
	VS_OUTPUT Out;

	float4 v = float4(In.Position.x, In.Position.y, 0.0f, 1.0f);

	Out.Position	= mul(g_viewProjMatrix,v);
	
	Out.Texcoord0 = In.Texcoord0 * 4.0f + g_viewportSize.zw;
	Out.Texcoord1 = In.Texcoord0 * 4.0f + float2(g_viewportSize.z, -g_viewportSize.w);
	Out.Texcoord2 = In.Texcoord0 * 4.0f + float2(-g_viewportSize.z, g_viewportSize.w);
	Out.Texcoord3 = In.Texcoord0 * 4.0f - g_viewportSize.zw;
	return Out;
}

