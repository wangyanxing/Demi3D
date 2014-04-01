#include "common.h"

attribute vec3  Position;
attribute vec2  Texcoord0;

varying vec2 vTexCoord0;
varying vec4 vTexCoord1;
varying vec4 vTexCoord2;
varying vec4 vTexCoord3;
varying vec4 vTexCoord4;
varying vec4 vTexCoord5;
varying vec4 vTexCoord6;

void main()
{
	vec4 v = vec4(Position.x, Position.y, 0.0, 1.0);
	gl_Position = g_viewProjMatrix * v;
	
    vTexCoord0.xy = Texcoord0 * 0.25 + vec2( g_viewportSize.z * -11.5,0.0);
	vTexCoord1.xy = Texcoord0 * 0.25 + vec2( g_viewportSize.z * -9.5,0.0);
	vTexCoord1.zw = Texcoord0 * 0.25 + vec2( g_viewportSize.z * -7.5,0.0);
	vTexCoord2.xy = Texcoord0 * 0.25 + vec2( g_viewportSize.z * -5.5,0.0);
	vTexCoord2.zw = Texcoord0 * 0.25 + vec2( g_viewportSize.z * -3.5,0.0);
	vTexCoord3.xy = Texcoord0 * 0.25 + vec2( g_viewportSize.z * -1.5,0.0);
	vTexCoord3.zw = Texcoord0 * 0.25 + vec2( g_viewportSize.z * 0.0,0.0);
	vTexCoord4.xy = Texcoord0 * 0.25 + vec2( g_viewportSize.z * 1.5,0.0);
	vTexCoord4.zw = Texcoord0 * 0.25 + vec2( g_viewportSize.z * 3.5,0.0);
	vTexCoord5.xy = Texcoord0 * 0.25 + vec2( g_viewportSize.z * 5.5,0.0);
	vTexCoord5.zw = Texcoord0 * 0.25 + vec2( g_viewportSize.z * 7.5,0.0);
	vTexCoord6.xy = Texcoord0 * 0.25 + vec2( g_viewportSize.z * 9.5,0.0);
	vTexCoord6.zw = Texcoord0 * 0.25 + vec2( g_viewportSize.z * 11.5,0.0);
}

