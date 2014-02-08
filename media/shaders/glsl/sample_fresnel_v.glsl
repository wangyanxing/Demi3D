
#include "common.h"

uniform float mRefractionRatio;
uniform float mFresnelBias;
uniform float mFresnelScale;
uniform float mFresnelPower;

attribute vec3  Position;
attribute vec3  Normal;

varying vec3  vReflect;
varying vec3  vRefract[3];
varying float vReflectionFactor;

void main()
{
	vec4 mvPosition = g_modelViewMatrix * vec4( Position.xyz, 1.0 );
	vec4 worldPosition = g_modelMatrix * vec4( Position.xyz, 1.0 );

	vec3 worldNormal = normalize( mat3( g_modelMatrix[0].xyz, g_modelMatrix[1].xyz, g_modelMatrix[2].xyz ) * Normal );

	vec3 I = worldPosition.xyz - g_eyePosition;

	vReflect = reflect( I, worldNormal );
	vRefract[0] = refract( normalize( I ), worldNormal, mRefractionRatio );
	vRefract[1] = refract( normalize( I ), worldNormal, mRefractionRatio * 0.99 );
	vRefract[2] = refract( normalize( I ), worldNormal, mRefractionRatio * 0.98 );
	vReflectionFactor = mFresnelBias + mFresnelScale * pow( 1.0 + dot( normalize( I ), worldNormal ), mFresnelPower );

	gl_Position = g_projMatrix * mvPosition;
}