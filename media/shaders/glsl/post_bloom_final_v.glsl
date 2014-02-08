
#include "common.h"

attribute vec3  Position;
attribute vec2  Texcoord0;

void main()
{
	gl_Position  = g_viewProjMatrix * vec4(Position.xy, 0.0, 1.0);
	gl_TexCoord[0].xy = Texcoord0.xy;
}