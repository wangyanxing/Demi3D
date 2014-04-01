
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
	gl_Position = g_viewProjMatrix * vec4(Position.xy, 0.0, 1.0);
	
	vTexCoord0.xy = Texcoord0 * 0.25 + vec2(0.0, g_viewportSize.w * -11.5);
	vTexCoord1.xy = Texcoord0 * 0.25 + vec2(0.0, g_viewportSize.w * -9.5);
	vTexCoord1.zw = Texcoord0 * 0.25 + vec2(0.0, g_viewportSize.w * -7.5);
	vTexCoord2.xy = Texcoord0 * 0.25 + vec2(0.0, g_viewportSize.w * -5.5);
	vTexCoord2.zw = Texcoord0 * 0.25 + vec2(0.0, g_viewportSize.w * -3.5);
	vTexCoord3.xy = Texcoord0 * 0.25 + vec2(0.0, g_viewportSize.w * -1.5);
	vTexCoord3.zw = Texcoord0 * 0.25 + vec2(0.0, g_viewportSize.w * 0.0);
	vTexCoord4.xy = Texcoord0 * 0.25 + vec2(0.0, g_viewportSize.w * 1.5);
	vTexCoord4.zw = Texcoord0 * 0.25 + vec2(0.0, g_viewportSize.w * 3.5);
	vTexCoord5.xy = Texcoord0 * 0.25 + vec2(0.0, g_viewportSize.w * 5.5);
	vTexCoord5.zw = Texcoord0 * 0.25 + vec2(0.0, g_viewportSize.w * 7.5);
	vTexCoord6.xy = Texcoord0 * 0.25 + vec2(0.0, g_viewportSize.w * 9.5);
	vTexCoord6.zw = Texcoord0 * 0.25 + vec2(0.0, g_viewportSize.w * 11.5);
}
