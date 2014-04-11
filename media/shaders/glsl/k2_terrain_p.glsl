
#include "common.h"

#ifndef _SHOW_LAYER_0
#	define _SHOW_LAYER_0 1
#endif

#ifndef _SHOW_LAYER_1
#	define _SHOW_LAYER_1 1
#endif

#if _SHOW_LAYER_0
uniform sampler2D diffuseMap_0;
uniform sampler2D normalMap_0;
#   ifndef DI_GLES2
        uniform sampler2D specularMap_0;
#   endif
#endif

#if _SHOW_LAYER_1
uniform sampler2D diffuseMap_1;
uniform sampler2D normalMap_1;
#   ifndef DI_GLES2
        uniform sampler2D specularMap_1;
#   endif
#endif

varying vec4 vPosition;
varying vec4 vColor0;
varying vec2 vTexcoord0;
varying vec3 vNormal;
varying vec3 vTangent;
varying vec3 vBinormal;
varying vec3 vViewDir;

void main()
{
	float fAlpha = vColor0.a;

#if _SHOW_LAYER_0
	vec4 diffuse0 = texture2D(diffuseMap_0,vTexcoord0.xy);
#else
	vec4 diffuse0 = vec4(1.0,1.0,1.0,1.0);
#endif

#if _SHOW_LAYER_1
	vec4 diffuse1 = texture2D(diffuseMap_1,vTexcoord0.xy);
#else
	vec4 diffuse1 = diffuse0;
#endif

	vec3 cDiffuseColor0 = diffuse0.rgb;
	vec3 cDiffuseColor1 = diffuse1.rgb;

	vec3 cDiffuseColor = mix(cDiffuseColor0.rgb, cDiffuseColor1.rgb, fAlpha) * vColor0.rgb;

#if _SHOW_LAYER_0
	vec4 cNormalmapColor0;
#   ifdef DI_GLES2
        cNormalmapColor0 = texture2D(normalMap_0, vTexcoord0);
#   else
        cNormalmapColor0.rgb = texture2D(normalMap_0, vTexcoord0).agb;
        cNormalmapColor0.a = texture2D(specularMap_0, vTexcoord0).g;
#   endif
#else
    vec4 cNormalmapColor0 = vec4(0.5, 0.5, 1, 1.0);
#endif

#if _SHOW_LAYER_1
	vec4 cNormalmapColor1;
#   ifdef DI_GLES2
        cNormalmapColor1 = texture2D(normalMap_1, vTexcoord0);
#   else
        cNormalmapColor1.rgb = texture2D(normalMap_1, vTexcoord0).agb;
        cNormalmapColor1.a = texture2D(specularMap_1, vTexcoord0).g;
#   endif
#else
    vec4 cNormalmapColor1 = vec4(0.5, 0.5, 1, 1.0);
#endif

	vec4 cNormalmapColor = mix(cNormalmapColor0, cNormalmapColor1, fAlpha);
	vec3 texNormal = vec3(cNormalmapColor.rgb * 2.0 - 1.0);
    
    mat3 rot = mat3(vTangent, vBinormal, vNormal);
    
#ifdef DI_GLES2
	vec3 finalNormal = normalize(vNormal);
#else
    vec3 finalNormal = normalize(texNormal * rot);
#endif
	
	vec3 vDiffuse = g_globalAmbient.rgb;
	vec3 vSpecular = vec3(0.0, 0.0, 0.0);
	
	for (int i = 0; i < g_numDirLights; ++i)
	{
		float NdotL = clamp(dot(finalNormal, -g_dirLightsDir[i].xyz),0.0,1.0); //152
		vec3 halfVec = normalize(-g_dirLightsDir[i].xyz + normalize(vViewDir));
		float dotHalf = clamp(dot(finalNormal, halfVec),0.0,1.0);
		float specWeight = clamp(pow(dotHalf, g_shininess),0.0,1.0);

		vDiffuse += g_dirLightsColor[i].xyz * NdotL;
		vSpecular += g_dirLightsColor[i].xyz * specWeight * cNormalmapColor.a;
	}

    vec3 vFinalColor = cDiffuseColor.rgb * vDiffuse + vSpecular;

    gl_FragColor.rgb = vFinalColor;
    gl_FragColor.a = 1.0;
}