#include "common.h"

attribute vec3  Position;

attribute vec4  Color;
varying   vec4	vColor;

attribute vec2  Texcoord0;
varying vec2 vTexCoord0;

void main()
{
    vec4 objPos = vec4(Position.xyz,1.0);
	
	gl_Position = g_modelViewProjMatrix * objPos;
	vColor = Color;
	
	vTexCoord0.xy = Texcoord0;
}