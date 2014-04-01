
#include "common.h"

attribute vec3  Position;
attribute vec2  Texcoord0;

uniform vec2  uvScale;

varying vec4  vPosition;
varying vec2  vTexCoord0;

void main()
{
	gl_Position = g_modelViewProjMatrix * vec4(Position.xyz, 1.0);
	vPosition = gl_Position;
	vTexCoord0.xy = uvScale * Texcoord0;
}
