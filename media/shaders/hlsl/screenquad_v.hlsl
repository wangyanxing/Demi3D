
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