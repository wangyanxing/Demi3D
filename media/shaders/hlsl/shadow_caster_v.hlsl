
#include <common.hlsl>

struct VS_INPUT
{
    float4 Position   	: POSITION;

#if defined( SKINNED)
    float4 BlendWeights : BLENDWEIGHT;
    int4   BlendIndices : BLENDINDICES;
#endif	
};

struct VS_OUTPUT
{
    float4 Position 	: POSITION;
    float2 Texcoord0  	: TEXCOORD0;
};

VS_OUTPUT vs_main(VS_INPUT In)
{
    float4 objPos = 0;
    GET_SPACE_POS

    VS_OUTPUT Out;

    Out.Position = mul(g_modelViewProjMatrix, objPos);
    Out.Position.xy += g_texelOffsets.zw * Out.Position.w;
    Out.Texcoord0.x = Out.Position.z;
    Out.Texcoord0.y = 0;

    return Out;
}