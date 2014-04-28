#include "common.h"
#include "transform.h"

attribute vec3  Position;
attribute vec3  Normal;
attribute vec3	Tangent;

attribute vec2  Texcoord0;
varying vec4 vTexcoord0;

#if defined( SKINNED )
attribute vec4  BlendWeights;
attribute vec4  BlendIndices;
#endif

attribute vec4  Color;
varying   vec4  vColor;

varying vec3 vNormal;
varying vec3 vViewDir;
varying vec3 vPosWorld;
varying vec3 vTangent;
varying vec3 vBinormal;

uniform float fSpeed;

void main()
{
	GET_SPACE_POS_NORMAL_TANGENT(objPos, objNormal, objTangent);
	
    vec4 viewPos = mul(g_modelViewMatrix,objPos);

	gl_Position = mul(g_projMatrix,viewPos);

	vColor = Color;
    vColor.a *= vColor.a;

    vTexcoord0.xy = Texcoord0 + vec2(fSpeed * g_time, fSpeed * g_time);
    vTexcoord0.zw = Texcoord0 * -1.5 + vec2(fSpeed * g_time, fSpeed * g_time);

	// world space normal
    vNormal = (g_modelMatrix * vec4(objNormal.xyz, 0.0)).xyz;
    vTangent = (g_modelMatrix * vec4(objTangent.xyz, 0.0)).xyz;
    vBinormal = cross(vNormal, vTangent);

	vPosWorld = (g_modelMatrix * objPos).xyz;
    vViewDir = g_eyePosition - vPosWorld;
}