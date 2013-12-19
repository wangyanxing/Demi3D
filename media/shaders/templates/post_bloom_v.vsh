
/********************************************************************
	日期:		2012-5-14
	文件名:		post_bloom_v.psh
	创建者:		王延兴
	
	描述:		模糊滤镜 垂直
*********************************************************************/

#include <common.hlsl>

struct VS_OUTPUT
{
	float4 Position  : POSITION;
	float2 Texcoord0 : TEXCOORD0;
	float4 Texcoord1 : TEXCOORD1;
	float4 Texcoord2 : TEXCOORD2;
	float4 Texcoord3 : TEXCOORD3;
	float4 Texcoord4 : TEXCOORD4;
	float4 Texcoord5 : TEXCOORD5;
	float4 Texcoord6 : TEXCOORD6;
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
	
	Out.Texcoord0	 = In.Texcoord0 + float2(0.0f, g_viewportSize.w * -11.5f);
	Out.Texcoord1.xy = In.Texcoord0 + float2(0.0f, g_viewportSize.w * -9.5f);
	Out.Texcoord1.zw = In.Texcoord0 + float2(0.0f, g_viewportSize.w * -7.5f);
	Out.Texcoord2.xy = In.Texcoord0 + float2(0.0f, g_viewportSize.w * -5.5f);
	Out.Texcoord2.zw = In.Texcoord0 + float2(0.0f, g_viewportSize.w * -3.5f);
	Out.Texcoord3.xy = In.Texcoord0 + float2(0.0f, g_viewportSize.w * -1.5f);
	Out.Texcoord3.zw = In.Texcoord0 + float2(0.0f, g_viewportSize.w * 0.0f);
	Out.Texcoord4.xy = In.Texcoord0 + float2(0.0f, g_viewportSize.w * 1.5f);
	Out.Texcoord4.zw = In.Texcoord0 + float2(0.0f, g_viewportSize.w * 3.5f);
	Out.Texcoord5.xy = In.Texcoord0 + float2(0.0f, g_viewportSize.w * 5.5f);
	Out.Texcoord5.zw = In.Texcoord0 + float2(0.0f, g_viewportSize.w * 7.5f);
	Out.Texcoord6.xy = In.Texcoord0 + float2(0.0f, g_viewportSize.w * 9.5f);
	Out.Texcoord6.zw = In.Texcoord0 + float2(0.0f, g_viewportSize.w * 11.5f);

	return Out;
}

