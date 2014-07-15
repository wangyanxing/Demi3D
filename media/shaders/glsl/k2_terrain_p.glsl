
#include "common.h"

uniform sampler2D diffuseMap_0;
uniform sampler2D diffuseMap_1;
uniform sampler2D normalMap_0;
uniform sampler2D normalMap_1;

#ifndef DI_GLES2
uniform sampler2D specularMap_0;
uniform sampler2D specularMap_1;
#endif

varying vec4 vColor0;
varying vec2 vTexcoord0;
varying vec3 vNormal;
varying vec3 vTangent;
varying vec3 vBinormal;
varying vec3 vViewDir;

void main()
{
	vec4 diffuse0 = texture2D(diffuseMap_0,vTexcoord0.xy);
	vec4 diffuse1 = texture2D(diffuseMap_1,vTexcoord0.xy);
	
	float fAlpha = vColor0.a * diffuse1.a;

	vec3 cDiffuseColor = mix(diffuse0.rgb, diffuse1.rgb, fAlpha) * vColor0.rgb;

#ifdef DI_GLES2
    vec4 cNormalmapColor0 = texture2D(normalMap_0, vTexcoord0);
    vec4 cNormalmapColor1 = texture2D(normalMap_1, vTexcoord0);
#else
    vec4 cNormalmapColor0;
    cNormalmapColor0.rgb = texture2D(normalMap_0, vTexcoord0).agb;
    cNormalmapColor0.a = texture2D(specularMap_0, vTexcoord0).g;
    
    vec4 cNormalmapColor1;
    cNormalmapColor1.rgb = texture2D(normalMap_1, vTexcoord0).agb;
    cNormalmapColor1.a = texture2D(specularMap_1, vTexcoord0).g;
#endif
    
	vec4 cNormalmapColor = mix(cNormalmapColor0, cNormalmapColor1, fAlpha);
	vec3 texNormal = vec3(cNormalmapColor.rgb * 2.0 - 1.0);
    
    mat3 rot = mat3(vTangent, vBinormal, vNormal);
    vec3 finalNormal = normalize(texNormal * rot);
    float specularValue = cNormalmapColor.a;

	
	vec3 vDiffuse = g_globalAmbient.rgb;
	vec3 vSpecular = vec3(0.0, 0.0, 0.0);
	
	for (int i = 0; i < g_numDirLights; ++i)
	{
		float NdotL = clamp(dot(finalNormal, -g_dirLightsDir[i].xyz),0.0,1.0); //152
		vec3 halfVec = normalize(-g_dirLightsDir[i].xyz + normalize(vViewDir));
		float dotHalf = clamp(dot(finalNormal, halfVec),0.0,1.0);
		float specWeight = clamp(pow(dotHalf, g_shininess),0.0,1.0);

		vDiffuse += g_dirLightsColor[i].xyz * NdotL;
		vSpecular += g_dirLightsColor[i].xyz * specWeight * specularValue;
	}

    vec3 vFinalColor = cDiffuseColor.rgb * vDiffuse + vSpecular;

    gl_FragColor.rgb = vFinalColor;
    gl_FragColor.a = 1.0;
}