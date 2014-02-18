#include "common.h"

attribute vec3  Position;
attribute vec3  Normal;
attribute vec2  Texcoord0;

uniform float scale;  // the amount to scale the noise texture by
uniform float scroll; // the amount by which to scroll the noise

varying vec4 projectionCoord;
varying vec2 noiseCoord;
varying vec3 vNormal;
varying vec3 vViewDir;

// Vertex program for fresnel reflections / refractions
void main()
{
	gl_Position = g_modelViewProjMatrix * vec4(Position, 1.0);
	
	// Projective texture coordinates, adjust for mapping
	mat4 scalemat = mat4(0.5, 0.0, 0.0, 0.0, 
                         0.0, -0.5, 0.0, 0.0,
                         0.0, 0.0, 0.5, 0.0,
                         0.5, 0.5, 0.5, 1.0);
	projectionCoord = scalemat * gl_Position;

	// Noise map coords
	noiseCoord.xy = (Texcoord0.xy + (g_time * scroll)) * scale;

	vec3 vPosWorld = (g_modelMatrix * vec4(Position, 1.0)).xyz;
	vNormal = (g_modelMatrix * vec4(Normal, 0.0)).xyz;
	vViewDir = vPosWorld - g_eyePosition.xyz;
}