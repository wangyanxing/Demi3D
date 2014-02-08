#include "common.h"

attribute vec3  Position;

void main()
{
	gl_Position = g_modelViewProjMatrix * vec4(Position, 1.0);
	gl_TexCoord[0].xyz = normalize(Position);
}