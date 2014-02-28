
#include <common.hlsl>

struct VS_OUTPUT
{
    float4 Position 	: POSITION;
    float2 Texcoord0  	: TEXCOORD0;
};

PS_OUTPUT ps_main(VS_OUTPUT In)
{
    PS_OUTPUT Out;
    Out.Color = float4(In.Texcoord0.x, 0, 0, 1);
    return Out;
}