
#include "common.h"

uniform vec4 v_WorldSizes;

attribute float Position;  // height
attribute vec4  Texcoord0;
attribute vec4  Texcoord1;
attribute vec4  Texcoord2; // vertex color

varying vec4 vPosition;
varying vec4 vColor0;
varying vec2 vTexcoord0;
varying vec3 vNormal;
varying vec3 vTangent;
varying vec3 vBinormal;
varying vec3 vViewDir;

void main()
{
	vec2 vTile      = vec2(Texcoord0.w, Texcoord1.w);
	
	vec4 vPosition	= vec4(vTile.x * v_WorldSizes.x, Position, vTile.y * v_WorldSizes.x, 1.0);
	vec3 vWorldPos	= (g_modelMatrix * vPosition).xyz;
	vec2 vUV        = vTile * v_WorldSizes.y;
	
    vec4 normal     = vec4(Texcoord0.xyz,1.0);
	vec4 tangent 	= vec4(Texcoord1.xyz,1.0);
	vec3 binormal   = cross(tangent.xyz, normal.xyz);

	vNormal         = (g_modelMatrix * normal).xyz;
	vTangent        = (g_modelMatrix * tangent).xyz;
	vBinormal       = (g_modelMatrix * vec4(binormal,1.0)).xyz;

	gl_Position     = g_modelViewProjMatrix * vPosition;
	vColor0         = Texcoord2; // vertex color
	vTexcoord0      = vUV.xy;
	vViewDir        = g_eyePosition - vWorldPos;
}
