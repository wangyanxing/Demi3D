
#include "common.h"

attribute vec3  Position;
attribute vec2  Texcoord0;

uniform   vec2  uvScale;

varying   vec4  vPosition;

void main()
{
	gl_Position = g_modelViewProjMatrix * vec4(Position.xyz, 1.0);
	vPosition = gl_Position;
	gl_TexCoord[0].xy = uvScale * Texcoord0;
}
