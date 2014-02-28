
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

#define CASCADE_COUNT_FLAG 4

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

    float  Depth        : TEXCOORD4;
};

// Data that we can read or derive from the surface shader outputs
struct SurfaceData
{
	float3 positionWorld;         // world space position
	float3 viewDirWorld;
    float3 normal;               // world space normal
    float4 albedo;
};

static SurfaceData gSurface;

void ComputeSurfaceDataFromGeometry(VS_OUTPUT input)
{
    gSurface.normal = normalize(input.Normal);
	gSurface.viewDirWorld = input.ViewDir;
	gSurface.positionWorld = input.PosWorld;
	
	#ifdef USE_MAP
		gSurface.albedo = tex2D( map, input.Texcoord0 );
		#ifdef GAMMA_INPUT
			gSurface.albedo.rgb *= gSurface.albedo.rgb;
		#endif
	#else
		gSurface.albedo = float4(1.0,1.0,1.0,1.0);
	#endif
	
	#ifdef USE_COLOR
		gSurface.albedo = gSurface.albedo * float4( In.Color, 1.0 );
	#endif	
	
	#ifdef ALPHATEST
		if ( gSurface.albedo.a * g_opacity < ALPHATEST ) discard;
	#endif
}

void AccumulateDirLight(float3 dir, float4 color,
                    inout float3 lit)
{
    float NdotL = max(dot(gSurface.normal, normalize(-dir)), 0.0);
	
	float3 litDiffuse = color.rgb * color.a * NdotL;
	lit += gSurface.albedo.rgb * litDiffuse;
}

void AccumulatePointLight(float3 position, float attenBegin,  float attenEnd, 
					float4 color, inout float3 lit)
{
	float3 directionToLight = position - gSurface.positionWorld;
    float distanceToLight = length(directionToLight);
	
	[branch] if (distanceToLight < attenEnd) {	
		float attenuation = linstep(attenEnd, attenBegin, distanceToLight);
        directionToLight *= rcp(distanceToLight);
		
		float NdotL = max(dot(gSurface.normal, normalize(directionToLight)), 0.0);
	    float3 litDiffuse = color.rgb * color.a * NdotL * attenuation;
		lit += gSurface.albedo.rgb * litDiffuse;
	}
}

void AccumulateSkyLight(float3 dir, float4 skycolor, float4 groundcolor, inout float3 lit){
    float NdotL = max(dot(gSurface.normal, normalize(dir)), 0.0);

    float skyDiffuseWeight = 0.5 * NdotL + 0.5;
    float3 color = lerp(groundcolor, skycolor, skyDiffuseWeight);
    lit += gSurface.albedo.rgb * color;
}

static const float4 vCascadeColorsMultiplier[8] =
{
    float4 (1.5f, 0.0f, 0.0f, 1.0f),
    float4 (0.0f, 1.5f, 0.0f, 1.0f),
    float4 (0.0f, 0.0f, 5.5f, 1.0f),
    float4 (1.5f, 0.0f, 5.5f, 1.0f),
    float4 (1.5f, 1.5f, 0.0f, 1.0f),
    float4 (1.0f, 1.0f, 1.0f, 1.0f),
    float4 (0.0f, 1.0f, 5.5f, 1.0f),
    float4 (0.5f, 3.5f, 0.75f, 1.0f)
};

PS_OUTPUT ps_main( VS_OUTPUT In )
{			
	PS_OUTPUT Out;
	
	ComputeSurfaceDataFromGeometry(In);
	
	float3 lit = float3(0,0,0);
	
	for(int i = 0; i < g_numDirLights; i++){
		AccumulateDirLight(g_dirLightsDir[i], g_dirLightsColor[i], lit);
	}
	
	for(int i = 0; i < g_numPointLights; i++){
		AccumulatePointLight(g_pointLightsPosition[i], 
			g_pointLightsAttenuation[i].x,g_pointLightsAttenuation[i].y, g_pointLightsColor[i], lit);
	}

#if 0
    [flatten] if (g_hasSkyLight){
        AccumulateSkyLight(g_skyLightDir, g_skyLightColor, g_groundColor, lit);
    }
#endif
	
	Out.Color.rgb = lit * g_diffuseColor.rgb;
	Out.Color.rgb += g_globalAmbient.rgb * g_ambientColor.rgb;	
	Out.Color.a = gSurface.albedo.a * g_opacity;

    // shadow mapping
    float4 vCurrentPixelDepth = In.Depth;
    float4 fComparison = (vCurrentPixelDepth > g_cascadeEyeSpaceDepths[0]);
    float4 fComparison2 = (vCurrentPixelDepth > g_cascadeEyeSpaceDepths[1]);
    float fIndex = dot( 
                    float4( CASCADE_COUNT_FLAG > 0,
                            CASCADE_COUNT_FLAG > 1, 
                            CASCADE_COUNT_FLAG > 2, 
                            CASCADE_COUNT_FLAG > 3)
                    , fComparison )
                 + dot( 
                    float4( CASCADE_COUNT_FLAG > 4,
                            CASCADE_COUNT_FLAG > 5,
                            CASCADE_COUNT_FLAG > 6,
                            CASCADE_COUNT_FLAG > 7)
                    , fComparison2 ) ;
                            
    int iCurrentCascadeIndex = (int)fIndex;
    iCurrentCascadeIndex = clamp(iCurrentCascadeIndex, 0, CASCADE_COUNT_FLAG - 1);

    float4 visCascadeColor = vCascadeColorsMultiplier[iCurrentCascadeIndex];
		
	// environment mapping
#if defined(USE_ENV_MAP)

	#ifdef ENV_MAP_REFRACT
		float3 reflVect = refract(normalize(In.ViewDir), gSurface.normal, refractionRatio);
	#else
		float3 reflVect = reflect(normalize(In.ViewDir), gSurface.normal);
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

    //Out.Color.rgb *= visCascadeColor.rgb;
	return Out;
}

