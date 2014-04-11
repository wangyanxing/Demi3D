
#include "common.h"
#include "transform.h"

attribute vec3  Position;
attribute vec3  Normal;

attribute vec2  Texcoord0;

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

varying vec4 vTexcoord0;
varying vec3 vNormal;
varying vec3 vViewDir;
varying vec3 vPosWorld;

uniform float cliffUVScale;

void main()
{
#if defined( USE_NORMALMAP )
    GET_SPACE_POS_NORMAL_TANGENT(objPos,objNormal,objTangent);
#else
    GET_SPACE_POS_NORMAL(objPos,objNormal);
#endif
	
	gl_Position = g_modelViewProjMatrix * objPos;
    
	vTexcoord0.xy = Texcoord0.xy;
    vTexcoord0.zw = (g_modelMatrix * objPos).xz * cliffUVScale;

    // world space normal
	vec4 model;
    vNormal    = (g_modelMatrix * vec4(objNormal,1.0)).xyz;
	vPosWorld  = (g_modelMatrix * objPos).xyz;
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