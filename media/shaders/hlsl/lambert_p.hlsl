
#include <common.hlsl>

#if USE_SHADOW
//#   define DEBUG_CSM
#   include <shadow.hlsl>
#endif

#ifdef USE_MAP
	uniform sampler2D map;
#endif

#ifdef USE_NORMALMAP
    uniform sampler2D normalMap;
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

#if USE_SHADOW
    float  Depth : TEXCOORD4;
    float4 ShadowLightspacePos : TEXCOORD5;
#endif

#if defined( USE_NORMALMAP )
    float3 Tangent      : TEXCOORD6;
    float3 Binormal     : TEXCOORD7;
#endif
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
	gSurface.viewDirWorld = input.ViewDir;
	gSurface.positionWorld = input.PosWorld;

#if defined( USE_NORMALMAP )
    float4 normMapColor;
    float4 nmTex = tex2D(normalMap, input.Texcoord0);
    normMapColor.rgb = nmTex.agb;
    normMapColor.a = nmTex.g;
    float3 texNormal = float3(normMapColor.rgb * 2.0f - 1.0f);
    float3x3 rot = float3x3(input.Tangent, input.Binormal, input.Normal);
    gSurface.normal = normalize(mul(texNormal,rot));
#else
    gSurface.normal = normalize(input.Normal);
#endif
	
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

PS_OUTPUT ps_main( VS_OUTPUT In )
{			
	PS_OUTPUT Out;
	
#if 1
	ComputeSurfaceDataFromGeometry(In);

#ifdef ALPHATEST
    if (gSurface.albedo.a * g_opacity < ALPHATEST) discard;
#endif
	
    float3 lit = float3(0,0,0);
    for (int i = 0; i < g_numDirLights; i++){
		AccumulateDirLight(g_dirLightsDir[i], g_dirLightsColor[i], lit);
	}
	
    for (int i = 0; i < g_numPointLights; i++){
		AccumulatePointLight(g_pointLightsPosition[i], 
			g_pointLightsAttenuation[i].x,g_pointLightsAttenuation[i].y, g_pointLightsColor[i], lit);
	}
#endif

#if 0
    [flatten] if (g_hasSkyLight){
        AccumulateSkyLight(g_skyLightDir, g_skyLightColor, g_groundColor, lit);
    }
#endif
    
    //float NdotL = max(dot(normalize(In.Normal), normalize(-g_dirLightsDir[0])), 0.0);
    //float3 lit = NdotL.xxx;

    // shadow mapping
#if USE_SHADOW
#   ifdef DEBUG_CSM
    half3 shadow = getCsmShadowFactor(In.ShadowLightspacePos, 0.0);
#   else
    half shadow = getCsmShadowFactor(In.ShadowLightspacePos, 0.0);
#   endif
#else
    half shadow = 1.0;
#endif
    /*
    half3 cascadeColor = half3(1, 1, 1);
    if (In.Depth < g_csmSplitPoints[0].x){
        cascadeColor = half3(1, 0, 0);
    }
    else if (In.Depth > g_csmSplitPoints[0].x && In.Depth < g_csmSplitPoints[1].x){
        cascadeColor = half3(0, 1, 0);
    }
    else if (In.Depth > g_csmSplitPoints[1].x && In.Depth < g_csmSplitPoints[2].x){
        cascadeColor = half3(0, 0, 1);
    }
    else if (In.Depth > g_csmSplitPoints[2].x && In.Depth < g_csmSplitPoints[3].x){
        cascadeColor = half3(0, 1, 1);
    }
	*/
    Out.Color.rgb = lit * g_diffuseColor.rgb * shadow;
	Out.Color.rgb += g_globalAmbient.rgb * g_ambientColor.rgb;	
	Out.Color.a = gSurface.albedo.a * g_opacity;

    //Out.Color.rgb *= cascadeColor;
		
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

	return Out;
}

