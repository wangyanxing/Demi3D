
/********************************************************************
	Copyright (c) 2012, 杭州电魂,	All rights reserved.
	
	日期:		2012-5-14
	文件名: 	screen_quad.vsh
	创建者:		王延兴
	
	描述:		标准屏幕矩形VP
*********************************************************************/

#include <globals.hlsl>

void vs_main
(
    in float4 inPos : POSITION,
    in float2 inUv0 : TEXCOORD0,

    out float4 pos : POSITION,
    out float2 uv0 : TEXCOORD0
)
{
	float4 v = float4(inPos.x, inPos.y, 0.0f, 1.0f);

	pos	= mul(g_viewProjMatrix,v);
	pos.x -= 0.001;
	pos.y += 0.001;
	uv0	= inUv0;
}