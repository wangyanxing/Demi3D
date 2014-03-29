#include "common.h"
#include "transform.h"

attribute vec3  Position;
attribute vec3  Normal;
#if defined( USE_NORMALMAP )
attribute vec3	Tangent;
#endif

#if defined( USE_MAP ) || defined( USE_NORMALMAP ) || defined( USE_SPECULARMAP )
attribute vec2  Texcoord0;
#endif

#if defined( SKINNED )
attribute vec4  BlendWeights;
attribute vec4  BlendIndices;
#endif

#if defined( USE_COLOR )
attribute vec4  Color;
varying   vec4  vColor;
#endif

varying vec3 vNormal;
varying vec3 vViewDir;
varying vec3 vPosWorld;

#if defined( USE_NORMALMAP )
varying vec3 vTangent;
varying vec3 vBinormal;
#endif

void main()
{
#if defined( USE_NORMALMAP )
    GET_SPACE_POS_NORMAL_TANGENT(objPos, objNormal, objTangent);
#else
	GET_SPACE_POS_NORMAL(objPos, objNormal);
#endif
	
	gl_Position = g_modelViewProjMatrix * objPos;

#if defined( USE_COLOR )
	vColor = Color;
#endif
	
#if defined( USE_MAP ) 
	gl_TexCoord[0] = vec4(Texcoord0,0.0,0.0);
#endif

	// world space normal
    vNormal   = (g_modelMatrix * vec4(objNormal,0.0)).xyz;
	vPosWorld = (g_modelMatrix * objPos).xyz;
	vViewDir  = g_eyePosition - vPosWorld;
    
#if defined( USE_NORMALMAP )
    vTangent  = (g_modelMatrix * vec4(objTangent, 0.0)).xyz;
    vBinormal = cross(vNormal, vTangent);
#endif
}