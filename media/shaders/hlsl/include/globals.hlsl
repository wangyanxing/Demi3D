
/********************************************************************
	File: 	    globals.hlsl
	Creator:    demiwangya
*********************************************************************/

#ifndef GLOBALS_HLSL
#define GLOBALS_HLSL

#define MAX_BONES			75
#define MAX_MODEL_MATS		75

#define MAX_LIGHTS			8

#define INSTANCE_NONE		0
#define INSTANCE_SHADER		1
#define INSTANCE_HARDWARE	2

uniform const float4x4      g_modelMatrix;
uniform const float4x4      g_viewMatrix;
uniform const float4x4      g_projMatrix;
uniform const float4x4      g_modelViewMatrix;
uniform const float4x4      g_modelViewProjMatrix;
uniform const float4x4      g_viewProjMatrix;
uniform const float4x4      g_texViewProjMatrix;

uniform const float4x3      g_boneMatrices[MAX_BONES];
uniform const float4x3      g_modelMatrices[MAX_MODEL_MATS];	//Instance

uniform const float3        g_eyePosition;
uniform const float3        g_eyeDirection;
uniform const float3        g_eyePositionObjSpace;

uniform const float2        g_farnearPlane;

uniform const float4        g_lightColor;
uniform const float         g_lightIntensity;
uniform const float3        g_lightDirection;
uniform const float3        g_lightPosition;

uniform const float         g_time;
uniform const float4		g_viewportSize;

uniform const float4        g_globalAmbient;

uniform const float4        g_ambientColor;
uniform const float3		g_diffuseColor;
uniform const float3		g_specularColor;
uniform const float 		g_opacity;
uniform const float 		g_shininess;

uniform const float4		g_texelOffsets;

uniform const int			g_numDirLights;
uniform const float4		g_dirLightsColor[MAX_LIGHTS];
uniform const float4		g_dirLightsDir[MAX_LIGHTS];

uniform const int			g_numPointLights;
uniform const float4		g_pointLightsColor[MAX_LIGHTS];
uniform const float4		g_pointLightsPosition[MAX_LIGHTS];
uniform const float4		g_pointLightsAttenuation[MAX_LIGHTS];

uniform const bool          g_hasSkyLight;
uniform const float4        g_skyLightColor;
uniform const float4        g_groundColor;
uniform const float3        g_skyLightDir;

#endif
