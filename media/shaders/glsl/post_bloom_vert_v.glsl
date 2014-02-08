
#include "common.h"

attribute vec3  Position;
attribute vec2  Texcoord0;

void main()
{
	gl_Position = g_viewProjMatrix * vec4(Position.xy, 0.0, 1.0);
	
	gl_TexCoord[0].xy = Texcoord0 * 0.25 + vec2(0.0, g_viewportSize.w * -11.5);
	gl_TexCoord[1].xy = Texcoord0 * 0.25 + vec2(0.0, g_viewportSize.w * -9.5);
	gl_TexCoord[1].zw = Texcoord0 * 0.25 + vec2(0.0, g_viewportSize.w * -7.5);
	gl_TexCoord[2].xy = Texcoord0 * 0.25 + vec2(0.0, g_viewportSize.w * -5.5);
	gl_TexCoord[2].zw = Texcoord0 * 0.25 + vec2(0.0, g_viewportSize.w * -3.5);
	gl_TexCoord[3].xy = Texcoord0 * 0.25 + vec2(0.0, g_viewportSize.w * -1.5);
	gl_TexCoord[3].zw = Texcoord0 * 0.25 + vec2(0.0, g_viewportSize.w * 0.0);
	gl_TexCoord[4].xy = Texcoord0 * 0.25 + vec2(0.0, g_viewportSize.w * 1.5);
	gl_TexCoord[4].zw = Texcoord0 * 0.25 + vec2(0.0, g_viewportSize.w * 3.5);
	gl_TexCoord[5].xy = Texcoord0 * 0.25 + vec2(0.0, g_viewportSize.w * 5.5);
	gl_TexCoord[5].zw = Texcoord0 * 0.25 + vec2(0.0, g_viewportSize.w * 7.5);
	gl_TexCoord[6].xy = Texcoord0 * 0.25 + vec2(0.0, g_viewportSize.w * 9.5);
	gl_TexCoord[6].zw = Texcoord0 * 0.25 + vec2(0.0, g_viewportSize.w * 11.5);
}
