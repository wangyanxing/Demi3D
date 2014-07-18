#include "common.h"
#include "transform.h"

attribute vec3  Position;
attribute vec3  Normal;

#if defined( SKINNED )
attribute vec4  BlendWeights;
attribute vec4  BlendIndices;
#endif

varying vec2 vDepth;

void main()
{
	GET_SPACE_POS_NORMAL(objPos, objNormal);
	
	gl_Position = g_modelViewProjMatrix * objPos;
    
    vDepth.x = (gl_Position.z - g_depthRange.x) * g_depthRange.w;
    //vDepth.x = gl_Position.z;
	vDepth.y = g_depthRange.w;
}