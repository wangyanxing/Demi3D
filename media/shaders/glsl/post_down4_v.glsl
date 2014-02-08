
#include "common.h"

attribute vec3  Position;
attribute vec2  Texcoord0;

void main()
{
	gl_Position = g_viewProjMatrix * vec4(Position.xy, 0.0, 1.0);
	
	//vec2 scaledUV = vec2(Texcoord0.x * 4.0, (1.0 - (1.0 - Texcoord0.y) * 4.0));
	vec2 scaledUV = vec2(Texcoord0.xy) ;
	
	gl_TexCoord[0].xy = scaledUV + g_viewportSize.zw;
    gl_TexCoord[1].xy = scaledUV + vec2(g_viewportSize.z, -g_viewportSize.w);
    gl_TexCoord[2].xy = scaledUV + vec2(-g_viewportSize.z, g_viewportSize.w);
    gl_TexCoord[3].xy = scaledUV - g_viewportSize.zw;
}