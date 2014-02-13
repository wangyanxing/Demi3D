
#include "common.h"

attribute vec3  Position;
attribute vec2  Texcoord0;

void main()
{
	gl_Position	= g_viewProjMatrix * vec4(Position.x, Position.y, 0.0f, 1.0f);
    gl_TexCoord[0].xy = Texcoord0.xy;
}