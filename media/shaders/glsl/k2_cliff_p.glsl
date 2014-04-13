
#include "common.h"

uniform sampler2D map;
uniform sampler2D terrainMap;

#ifdef USE_NORMALMAP
uniform sampler2D normalMap;
uniform sampler2D specularMap;
uniform sampler2D terrainNormalMap;

#   ifndef DI_GLES2
uniform sampler2D terrainSpecularMap;
#   endif

varying vec3 vTangent;
varying vec3 vBinormal;
varying vec3 vTerrainTangent;
varying vec3 vTerrainBinormal;
#endif 

varying vec4 vTexcoord0;
varying vec3 vNormal;
varying vec3 vViewDir;
varying vec3 vPosWorld;

// Data that we can read or derive from the surface shader outputs
struct SurfaceData
{
    vec3 positionWorld;         // world space position
	vec3 viewDirWorld;
    vec3 normal;               // world space normal
    vec4 albedo;
    vec3 specularAmount;        // Treated as a multiplier on albedo
};

SurfaceData gSurface;

void ComputeSurfaceDataFromGeometry()
{
    gSurface.positionWorld = vPosWorld;
    gSurface.viewDirWorld = vViewDir;

    vec4 cliffDiffuse = texture2D(map, vTexcoord0.xy);
    vec4 terrainDiffuse = texture2D(terrainMap, vTexcoord0.zw);
    gSurface.albedo.rgb = mix(terrainDiffuse.rgb, cliffDiffuse.rgb, cliffDiffuse.a);
    gSurface.albedo.a = 1.0;

#if defined( USE_NORMALMAP )
    
#   ifdef DI_GLES2
    vec4 cCliffNormalmapColor = texture2D(normalMap, vTexcoord0.xy);
    vec4 cTerrainNormalmapColor = texture2D(terrainNormalMap, vTexcoord0.zw);
#   else
    vec4 cCliffNormalmapColor;
    cCliffNormalmapColor.rgb = texture2D(normalMap, vTexcoord0.xy).agb;
    cCliffNormalmapColor.a = texture2D(specularMap, vTexcoord0.xy).g;
    
    vec4 cTerrainNormalmapColor;
    cTerrainNormalmapColor.rgb = texture2D(terrainNormalMap, vTexcoord0.zw).agb;
    cTerrainNormalmapColor.a = texture2D(terrainSpecularMap, vTexcoord0.zw).g;
#   endif

    gSurface.specularAmount.rgb = vec3(mix(cTerrainNormalmapColor.a, cCliffNormalmapColor.a, cliffDiffuse.a));

    vec3 vTexNormalCliff = vec3(cCliffNormalmapColor.rgb * 2.0 - 1.0);
    vec3 vTexNormalTerrain = vec3(cTerrainNormalmapColor.rgb * 2.0 - 1.0);

    mat3 rotCliff = mat3(vTangent, vBinormal, vNormal);
    mat3 rotTerrain = mat3(vTerrainTangent, vTerrainBinormal, vNormal);
    vec3 vNormalTerrain = vTexNormalTerrain * rotTerrain;
    vec3 vNormalCliff = vTexNormalCliff * rotCliff;
    gSurface.normal = normalize(mix(vNormalTerrain, vNormalCliff, cliffDiffuse.a));
#else
    gSurface.normal = normalize(vNormal);
#endif

	#ifdef GAMMA_INPUT
    gSurface.albedo.rgb *= gSurface.albedo.rgb;
	#endif
}

void AccumulatePhong(vec3 normal,
                     vec3 lightDir,
                     vec3 viewDir,
                     vec3 lightContrib,
                     inout vec3 litDiffuse,
                     inout vec3 litSpecular)
{
    float NdotL = max(dot(normal, lightDir), 0.0);
    vec3 halfVec = normalize(lightDir + viewDir);
    float dotHalf = max(dot(normal, halfVec), 0.0);
    float specWeight = max(pow(dotHalf, g_shininess), 0.0);

#ifdef USE_SCHLICK_FRESNEL
    float specularNormalization = (g_shininess + 2.0001) / 8.0;
    vec3 schlick = gSurface.specularAmount + vec3(1.0 - gSurface.specularAmount)
        * pow(1.0 - dot(lightDir, halfVec), 5.0);

    vec3 specular = schlick * specWeight * NdotL * specularNormalization;
#else
    vec3 specular = gSurface.specularAmount * specWeight;
#endif

    litDiffuse += lightContrib * NdotL;
    litSpecular += lightContrib * specular;
}

void AccumulateDirLight(vec3 dir, vec4 color, 
    inout vec3 diffuse, inout vec3 specular) {

    vec3 litDiffuse = vec3(0.0, 0.0, 0.0);
    vec3 litSpecular = vec3(0.0, 0.0, 0.0);
	
    AccumulatePhong(gSurface.normal, normalize(-dir), normalize(gSurface.viewDirWorld),
        color.rgb * color.a, litDiffuse, litSpecular);
	
    diffuse += litDiffuse;
    specular += litSpecular;
}

void AccumulatePointLight(vec3 position, float attenBegin, float attenEnd, vec4 color, 
    inout vec3 diffuse, inout vec3 specular) {

    vec3 directionToLight = position - gSurface.positionWorld;
    float distanceToLight = length(directionToLight);
	
    vec3 litDiffuse = vec3(0.0, 0.0, 0.0);
    vec3 litSpecular = vec3(0.0, 0.0, 0.0);
	
    if (distanceToLight < attenEnd) {
		float attenuation = linstep(attenEnd, attenBegin, distanceToLight);
        directionToLight /= distanceToLight;
		
        AccumulatePhong(gSurface.normal, directionToLight, normalize(gSurface.viewDirWorld),
			attenuation * color.rgb * color.a, litDiffuse, litSpecular);
		
        diffuse += litDiffuse;
        specular += litSpecular;
	}
}

void main()
{
	ComputeSurfaceDataFromGeometry();
	
#ifdef ALPHATEST
    if (gSurface.albedo.a * g_opacity < ALPHATEST) discard;
#endif

    vec3 vDiffuse = g_globalAmbient.rgb;
    vec3 vSpecular = vec3(0.0);
	
	for(int i = 0; i < g_numDirLights; i++){
        AccumulateDirLight(g_dirLightsDir[i].xyz, g_dirLightsColor[i],
            vDiffuse, vSpecular);
	}
	
#if 0
	for(int i = 0; i < g_numPointLights; i++){
		AccumulatePointLight(g_pointLightsPosition[i].xyz,
            g_pointLightsAttenuation[i].x, g_pointLightsAttenuation[i].y,
            g_pointLightsColor[i], vDiffuse, vSpecular);
	}
#endif

    vec3 vFinalColor = gSurface.albedo.rgb * vDiffuse + vSpecular;
	
    gl_FragColor.rgb = vFinalColor;
    gl_FragColor.a = gSurface.albedo.a * g_opacity;

#ifdef GAMMA_OUTPUT
	gl_FragColor.rgb = sqrt( gl_FragColor.xyz );
#endif
}