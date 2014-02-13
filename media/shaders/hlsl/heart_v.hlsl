
#include <globals.hlsl>

void vs_main
(
in float4 inPos : POSITION,

out float4 pos : POSITION,
out float4 uv0 : TEXCOORD0
)
{
    float4 v = float4(inPos.x, inPos.y, 0.0f, 1.0f);
    pos = mul(g_viewProjMatrix, v);

    uv0 = pos;

    pos.x -= 0.001;
    pos.y += 0.001;
}