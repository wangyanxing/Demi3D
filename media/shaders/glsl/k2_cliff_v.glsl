
#include "common.h"

attribute vec3  Position;
attribute vec3  Normal;

#if defined( USE_MAP )
attribute vec2  Texcoord0;
varying vec4 vTexCoord0;
#endif

#if defined( USE_NORMALMAP )
attribute vec4 Tangent;

varying vec3 vTangent;
varying vec3 vBinormal;
varying vec3 vTerrainTangent;
varying vec3 vTerrainBinormal;
#endif

#if defined( USE_COLOR )
attribute vec4  Color;
varying   vec4	vColor;
#endif

varying vec3 vNormal;
varying vec3 vViewDir;
varying vec3 vPosWorld;

uniform float cliffUVScale;

void main()
{
	vec4 objPos = 0.0;
	vec3 objNormal = 0.0;
#if defined( USE_NORMALMAP )
    vec3 objTangent = 0.0;
    GET_SPACE_POS_NORMAL_TANGENT
#else
    GET_SPACE_POS_NORMAL
#endif
	
	gl_Position = g_modelViewProjMatrix * objPos;
    
	vTexcoord0.xy = Texcoord0.xy;
    vTexcoord0.zw = (g_modelMatrix * objPos).xz * cliffUVScale;

    // world space normal
	vec4 model;
    vNormal    = (g_modelMatrix * objNormal).xyz;
	vPosWorld  = g_modelMatrix * objPos;
    vViewDir   = g_eyePosition - vPosWorld;

#if defined( USE_NORMALMAP )
    vTangent = (g_modelMatrix * vec4(objTangent.xyz, 0.0)).xyz;
    vBinormal = cross(vNormal, vTangent);
    vec3 vTerrainTan = vec3(1.0, 0.0, 0.0);
    vTerrainTan -= vNormal * dot(vNormal, vTerrainTan);
    vTerrainTangent = normalize(vTerrainTan);
    vTerrainBinormal = cross(vTerrainTangent, vNormal);
#endif
}