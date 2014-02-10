#include "common.h"

varying vec3 vNormal;
varying vec3 vViewDir;
varying vec3 vPosWorld;

// options
#define USE_SCHLICK_FRESNEL

#if defined( USE_BUMPMAP ) || defined( USE_NORMALMAP )
varying vec3 Tangent;
varying vec3 Binormal;
#endif

#if defined( USE_COLOR )
varying vec4 vColor;
#endif

#ifdef USE_MAP
uniform sampler2D map;
#endif

#ifdef USE_SPECULARMAP
uniform sampler2D specularMap;
#endif

#ifdef USE_ENV_MAP
uniform samplerCube envMap;
uniform float       reflectivity;
#	ifdef ENV_MAP_REFRACT
		uniform float  refractionRatio;
#	endif
#endif

// Data that we can read or derive from the surface shader outputs
struct SurfaceData
{
    vec3 positionWorld;       // world space position
	vec3 viewDirWorld;
    vec3 normal;              // world space normal
    vec4 albedo;
    vec3 specularAmount;      // Treated as a multiplier on albedo
};

SurfaceData gSurface;

void ComputeSurfaceDataFromGeometry()
{
    gSurface.normal = normalize(vNormal);
	gSurface.viewDirWorld = vViewDir;
	gSurface.positionWorld = vPosWorld;
	
	#ifdef USE_MAP
		gSurface.albedo = texture2D( map, gl_TexCoord[0].xy );
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
	
	#ifdef USE_SPECULARMAP
		gSurface.specularAmount = texture2D( specularMap, gl_TexCoord[0].xy ).rgb * g_specularColor.rgb;
	#else
		gSurface.specularAmount = g_specularColor.rgb;
	#endif
}

void AccumulatePhong(vec3 normal,
                         vec3 lightDir,
                         vec3 viewDir,
                         vec3 lightContrib,
                         inout vec3 litDiffuse,
                         inout vec3 litSpecular) {
    float NdotL = max(dot(normal, lightDir), 0.0);
	
	vec3 halfVec = normalize(lightDir + viewDir);
	float dotHalf = max(dot(normal, halfVec), 0.0);
	float specWeight = max(pow(dotHalf, g_shininess), 0.0);
		
#ifdef USE_SCHLICK_FRESNEL
	float specularNormalization = ( g_shininess + 2.0001 ) / 8.0;
	vec3 schlick = gSurface.specularAmount + vec3( 1.0 - gSurface.specularAmount ) 
					* pow( 1.0 - dot( lightDir, halfVec ), 5.0 );
					
	vec3 specular = schlick * specWeight * NdotL * specularNormalization;
#else
	vec3 specular = gSurface.specularAmount * specWeight;
#endif
		
	litDiffuse += lightContrib * NdotL * gSurface.albedo.rgb;
    litSpecular += lightContrib * specular;
}

void AccumulateDirLight(vec3 dir, vec4 color, inout vec3 lit){
	vec3 litDiffuse = vec3(0.0);
    vec3 litSpecular = vec3(0.0);
	
    AccumulatePhong(gSurface.normal, normalize(dir), normalize(gSurface.viewDirWorld),
        color.rgb * color.a, litDiffuse, litSpecular);
	
    lit += litDiffuse + litSpecular;
}

void AccumulatePointLight(vec3 position, float attenBegin,  float attenEnd, vec4 color, inout vec3 lit) {
	vec3 directionToLight = position - gSurface.positionWorld;
    float distanceToLight = length(directionToLight);
	
    vec3 litDiffuse = vec3(0.0);
    vec3 litSpecular = vec3(0.0);
	
	if (distanceToLight < attenEnd) {
		float attenuation = linstep(attenEnd, attenBegin, distanceToLight);
        directionToLight /= distanceToLight;
		
		AccumulatePhong(gSurface.normal, directionToLight, normalize(gSurface.viewDirWorld),
			attenuation * color.rgb * color.a, litDiffuse, litSpecular);
		
		lit += litDiffuse + litSpecular;
	}
}

void AccumulateSkyLight(vec3 dir, vec4 skycolor, vec4 groundcolor, inout vec3 lit){
	vec3 litDiffuse = vec3(0.0);
	vec3 litSpecular = vec3(0.0);
	
	float NdotL = dot(gSurface.normal, normalize(dir));

	if (NdotL > 0.0f) {
		float skyDiffuseWeight = 0.5 * NdotL + 0.5;
		litDiffuse = mix( groundcolor, skycolor, skyDiffuseWeight );
		
		vec3 r = reflect(dir, gSurface.normal);
        float RdotV = 0.5 * dot(r, normalize(gSurface.viewDirWorld)) + 0.5;
        float specularSky = max(pow(RdotV, g_shininess), 0.0);
		
		vec3 groundDir = -dir;
		r = reflect(groundDir, gSurface.normal);
        float RdotVGround = 0.5 * dot(r, normalize(gSurface.viewDirWorld)) + 0.5;
        float specularGround = max(pow(RdotVGround, g_shininess), 0.0);
		
		float dotGround = dot( gSurface.normal, groundDir );
		float specularNormalization = ( g_shininess + 2.0001 ) / 8.0;
		
		vec3 schlickSky = gSurface.specularAmount + vec3( 1.0 - gSurface.specularAmount ) * pow( 1.0 - RdotV, 5.0 );
		vec3 schlickGround = gSurface.specularAmount + vec3( 1.0 - gSurface.specularAmount ) * pow( 1.0 - RdotVGround, 5.0 );
		litSpecular += litDiffuse * specularNormalization * ( schlickSky * specularSky * max( NdotL, 0.0 ) + schlickGround * specularGround * max( dotGround, 0.0 ) );
	
		lit += gSurface.albedo.rgb * litDiffuse + gSurface.specularAmount * litSpecular;
	}
}

void main()
{
	ComputeSurfaceDataFromGeometry();
	
	vec3 lit = vec3(0.0,0.0,0.0);
	
	for(int i = 0; i < g_numDirLights; i++){
		AccumulateDirLight(g_dirLightsDir[i].xyz, g_dirLightsColor[i], lit);
	}
	
	for(int i = 0; i < g_numPointLights; i++){
		AccumulatePointLight(g_pointLightsPosition[i].xyz,
			g_pointLightsAttenuation[i].x,g_pointLightsAttenuation[i].y, g_pointLightsColor[i], lit);
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
