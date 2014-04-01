#include "common.h"

varying vec3 vNormal;
varying vec3 vViewDir;
varying vec3 vPosWorld;

#if defined( USE_COLOR )
varying vec4 vColor;
varying vec4 vTexCoord0;
#endif

#ifdef USE_MAP
uniform sampler2D map;
#endif

#ifdef USE_ENV_MAP
uniform samplerCube envMap;
uniform float       reflectivity;
#	ifdef ENV_MAP_REFRACT
		uniform float  refractionRatio;
#	endif
#endif


// Data that we can read or derive from the surface shader outputs
struct SurfaceData{
	vec3 positionWorld;   // world space position
	vec3 viewDirWorld;
    vec3 normal;          // world space normal
    vec4 albedo;
};

SurfaceData gSurface;

void ComputeSurfaceDataFromGeometry() {
    gSurface.normal = normalize(vNormal);
	gSurface.viewDirWorld = vViewDir;
	gSurface.positionWorld = vPosWorld;
	
	#ifdef USE_MAP
		gSurface.albedo = texture2D( map, vTexCoord0.xy );
		#ifdef GAMMA_INPUT
			gSurface.albedo.rgb *= gSurface.albedo.rgb;
		#endif
	#else
		gSurface.albedo = vec4(1.0,1.0,1.0,1.0);
	#endif
	
	#ifdef USE_COLOR
		gSurface.albedo = gSurface.albedo * vec4( vColor, 1.0 );
	#endif	
	
	#ifdef ALPHATEST
		if ( gSurface.albedo.a * g_opacity < ALPHATEST ) discard;
	#endif
}

void AccumulateDirLight(vec3 dir,vec4 color, inout vec3 lit){

	float NdotL = max(dot(gSurface.normal, normalize(-dir)), 0.0);
	
	vec3 litDiffuse = color.rgb * color.a * NdotL;
	lit += gSurface.albedo.rgb * litDiffuse;
}

void AccumulatePointLight(vec3 position, float attenBegin,  float attenEnd, 
					vec4 color, inout vec3 lit){
					
	vec3 directionToLight = position - gSurface.positionWorld;
    float distanceToLight = length(directionToLight);
	
	if (distanceToLight < attenEnd) {	
		float attenuation = linstep(attenEnd, attenBegin, distanceToLight);
        directionToLight /= distanceToLight;
		
		float NdotL = dot(gSurface.normal, normalize(directionToLight));
		if (NdotL > 0.0) {
			vec3 litDiffuse = color.rgb * color.a * NdotL * attenuation;
			lit += gSurface.albedo.rgb * litDiffuse;
		}
	}
}

void AccumulateSkyLight(vec3 dir, vec4 skycolor, vec4 groundcolor, inout vec3 lit){
	float NdotL = max(dot(gSurface.normal, normalize(dir)), 0.0);

	float skyDiffuseWeight = 0.5 * NdotL + 0.5;
	vec3 color = mix( groundcolor.rgb, skycolor.rgb, skyDiffuseWeight );
	lit += gSurface.albedo.rgb * color;
}

void main(){

	ComputeSurfaceDataFromGeometry();
	
	vec3 lit = vec3(0.0,0.0,0.0);
	
	for(int i = 0; i < g_numDirLights; i++){
		AccumulateDirLight(g_dirLightsDir[i].xyz, g_dirLightsColor[i], lit);
	}
	
	for(int i = 0; i < g_numPointLights; i++){
		AccumulatePointLight(g_pointLightsPosition[i].xyz,
			g_pointLightsAttenuation[i].x,g_pointLightsAttenuation[i].y, g_pointLightsColor[i], lit);
	}
	
	if (g_hasSkyLight == 1){
		AccumulateSkyLight(g_skyLightDir, g_skyLightColor, g_groundColor, lit);
	}
	
	gl_FragColor.rgb = lit * g_diffuseColor.rgb;
	gl_FragColor.rgb += g_globalAmbient.rgb * g_ambientColor.rgb;	
	gl_FragColor.a = gSurface.albedo.a * g_opacity;
	
	// environment mapping
#if defined(USE_ENV_MAP)

	#ifdef ENV_MAP_REFRACT
		vec3 reflVect = refract(normalize(vViewDir), gSurface.normal, refractionRatio);
	#else
		vec3 reflVect = reflect(normalize(vViewDir), gSurface.normal);
	#endif
	
	vec4 cubeColor = textureCube( envMap, reflVect.xyz );
	
	#ifdef GAMMA_INPUT
		cubeColor.rgb *= cubeColor.rgb;
	#endif
	
	#if defined( ENV_MAP_ADD )
		gl_FragColor.rgb += cubeColor.rgb * reflectivity;
	#elif defined( ENV_MAP_MIX )
		gl_FragColor.rgb = mix( gl_FragColor.rgb, cubeColor.rgb, reflectivity );
	#else
		gl_FragColor.rgb = mix( gl_FragColor.rgb, gl_FragColor.rgb * cubeColor.rgb, reflectivity );
	#endif
#endif

#ifdef GAMMA_OUTPUT
	gl_FragColor.rgb = sqrt( gl_FragColor.xyz );
#endif
}
