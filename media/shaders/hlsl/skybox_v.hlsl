
/********************************************************************
	date:		2012-4-19
	file: 		skybox.vsh
	creator:	Wang Yanxing
*********************************************************************/

#include <globals.hlsl>

void vs_main
(
    in float4 inPos : POSITION,

    out float4 pos : POSITION,
    out float3 uv0 : TEXCOORD0
)
{
	pos	= mul(g_modelViewProjMatrix,inPos);
	uv0 = normalize(inPos);
}