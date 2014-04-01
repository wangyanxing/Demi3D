#include "common.h"

attribute vec3  Position;
varying vec3 vTexCoord0;

void main()
{
	gl_Position = g_modelViewProjMatrix * vec4(Position, 1.0);
	vTexCoord0.xyz = normalize(Position);
}