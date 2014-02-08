
#include <common.hlsl>

#ifdef USE_MAP
	uniform sampler2D map;
#endif

#ifdef USE_ENV_MAP
	uniform samplerCUBE envMap;
	uniform float		reflectivity;
	#ifdef ENV_MAP_REFRACT
		uniform float 	refractionRatio;
	#endif
#endif

struct VS_OUTPUT
{
	float4 Position 	: POSITION;
	
#if defined( USE_COLOR )
	half4  Color		: COLOR;
#endif

#if defined( USE_MAP ) || defined( USE_BUMPMAP ) || defined( USE_NORMALMAP )
	float2 Texcoord0  	: TEXCOORD0;
#endif

	float3 Normal       : TEXCOORD1;
	
	float3 ViewDir 		: TEXCOORD2;
	
	float3 PosWorld		: TEXCOORD3;
};

// Data that we can read or derive from the surface shader outputs
struct SurfaceData
{
	float3 positionWorld;         // world space position
	float3 viewDirWorld;
    float3 normal;               // world space normal
    float4 albedo;
};

SurfaceData ComputeSurfaceDataFromGeometry(VS_OUTPUT input)
{
    SurfaceData surface;
    surface.normal = normalize(input.Normal);
	surface.viewDirWorld = input.ViewDir;
	surface.positionWorld = input.PosWorld;
	
	#ifdef USE_MAP
		surface.albedo = tex2D( map, input.Texcoord0 );
		#ifdef GAMMA_INPUT
			surface.albedo.rgb *= surface.albedo.rgb;
		#endif
	#else
		surface.albedo = float4(1.0,1.0,1.0,1.0);
	#endif
	
	#ifdef USE_COLOR
		surface.albedo = surface.albedo * float4( In.Color, 1.0 );
	#endif	
	
	#ifdef ALPHATEST
		if ( surface.albedo.a * g_opacity < ALPHATEST ) discard;
	#endif
	
    return surface;
}

void AccumulateDirLight(SurfaceData surface, float3 dir, float4 color,
                    inout float3 lit)
{
	float NdotL = dot(surface.normal, normalize(dir));
	
	[flatten] if (NdotL > 0.0f) {
		float3 litDiffuse = color.rgb * color.a * NdotL;
		lit += surface.albedo.rgb * litDiffuse;
	}
}

void AccumulatePointLight(SurfaceData surface, float3 position, float attenBegin,  float attenEnd, 
					float4 color, inout float3 lit)
{
	float3 directionToLight = position - surface.positionWorld;
    float distanceToLight = length(directionToLight);
	
	[branch] if (distanceToLight < attenEnd) {	
		float attenuation = linstep(attenEnd, attenBegin, distanceToLight);
        directionToLight *= rcp(distanceToLight);
		
		float NdotL = dot(surface.normal, normalize(directionToLight));
		[flatten] if (NdotL > 0.0f) {
			float3 litDiffuse = color.rgb * color.a * NdotL * attenuation;
			lit += surface.albedo.rgb * litDiffuse;
		}
	}
}

PS_OUTPUT ps_main( VS_OUTPUT In )
{			
	PS_OUTPUT Out;
	
	SurfaceData surface = ComputeSurfaceDataFromGeometry(In);
	
	float3 lit = float3(0,0,0);
	
	for(int i = 0; i < g_numDirLights; i++){
		AccumulateDirLight(surface, g_dirLightsDir[i], g_dirLightsColor[i], lit);
	}
	
	for(int i = 0; i < g_numPointLights; i++){
		AccumulatePointLight(surface, g_pointLightsPosition[i], 
			g_pointLightsAttenuation[i].x,g_pointLightsAttenuation[i].y, g_pointLightsColor[i], lit);
	}
	
	Out.Color.rgb = lit * g_diffuseColor.rgb;
	Out.Color.rgb += g_globalAmbient.rgb * g_ambientColor.rgb;	
	Out.Color.a = surface.albedo.a * g_opacity;
		
	// environment mapping
#if defined(USE_ENV_MAP)

	#ifdef ENV_MAP_REFRACT
		float3 reflVect = refract(normalize(In.ViewDir), surface.normal, refractionRatio);
	#else
		float3 reflVect = reflect(normalize(In.ViewDir), surface.normal);
	#endif
	
	float4 cubeColor = texCUBE( envMap, reflVect.xyz );
	
	#ifdef GAMMA_INPUT
		cubeColor.rgb *= cubeColor.rgb;
	#endif
	
	#if defined( ENV_MAP_ADD )
		Out.Color.rgb += cubeColor.rgb * reflectivity;
	#elif defined( ENV_MAP_MIX )
		Out.Color.rgb = lerp( Out.Color.rgb, cubeColor.rgb, reflectivity );
	#else
		Out.Color.rgb = lerp( Out.Color.rgb, Out.Color.rgb * cubeColor.rgb, reflectivity );
	#endif
#endif

#ifdef GAMMA_OUTPUT
	Out.Color.rgb = sqrt( Out.Color.xyz );
#endif

	//Out.Color.rgb = surface.normal.rgb;
	//Out.Color.a = 1.0;
	return Out;
}

