#include "common.h"

attribute vec3  Position;
attribute vec2  Texcoord0;

void main()
{
	vec4 v = vec4(Position.x, Position.y, 0.0, 1.0f);
	gl_Position = g_viewProjMatrix * v;
	
    gl_TexCoord[0].xy = Texcoord0 * 0.25 + vec2( g_viewportSize.z * -11.5,0.0);
	gl_TexCoord[1].xy = Texcoord0 * 0.25 + vec2( g_viewportSize.z * -9.5,0.0);
	gl_TexCoord[1].zw = Texcoord0 * 0.25 + vec2( g_viewportSize.z * -7.5,0.0);
	gl_TexCoord[2].xy = Texcoord0 * 0.25 + vec2( g_viewportSize.z * -5.5,0.0);
	gl_TexCoord[2].zw = Texcoord0 * 0.25 + vec2( g_viewportSize.z * -3.5,0.0);
	gl_TexCoord[3].xy = Texcoord0 * 0.25 + vec2( g_viewportSize.z * -1.5,0.0);
	gl_TexCoord[3].zw = Texcoord0 * 0.25 + vec2( g_viewportSize.z * 0.0,0.0);
	gl_TexCoord[4].xy = Texcoord0 * 0.25 + vec2( g_viewportSize.z * 1.5,0.0);
	gl_TexCoord[4].zw = Texcoord0 * 0.25 + vec2( g_viewportSize.z * 3.5,0.0);
	gl_TexCoord[5].xy = Texcoord0 * 0.25 + vec2( g_viewportSize.z * 5.5,0.0);
	gl_TexCoord[5].zw = Texcoord0 * 0.25 + vec2( g_viewportSize.z * 7.5,0.0);
	gl_TexCoord[6].xy = Texcoord0 * 0.25 + vec2( g_viewportSize.z * 9.5,0.0);
	gl_TexCoord[6].zw = Texcoord0 * 0.25 + vec2( g_viewportSize.z * 11.5,0.0);
}

