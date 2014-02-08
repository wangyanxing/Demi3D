
#include <common.hlsl>

#ifdef USE_MAP
	uniform sampler2D map;
#endif

#ifdef USE_SPECULARMAP
	uniform sampler2D specularMap;
#endif

struct VS_OUTPUT
{
	float4 Position 	: POSITION;
	
#if defined( USE_COLOR )
	half4  Color		: COLOR;
#endif

#if defined( USE_MAP ) || defined( USE_BUMPMAP ) || defined( USE_NORMALMAP ) || defined( USE_SPECULARMAP )
	float2 Texcoord0  	: TEXCOORD0;
#endif

#if defined( USE_LIGHTMAP )
	float2 Texcoord1	: TEXCOORD1;
#endif

	float3 Normal       : TEXCOORD2;
	
    float3 ViewDir	 	: TEXCOORD3;
	
	float3 PosWorld 	: TEXCOORD4;

#if defined( USE_BUMPMAP ) || defined( USE_NORMALMAP )
	float3 Tangent		: TEXCOORD5;
	float3 Binormal		: TEXCOORD6;
#endif
};

// Data that we can read or derive from the surface shader outputs
struct SurfaceData
{
    float3 positionWorld;         // world space position
	float3 viewDirWorld;
    float3 normal;               // world space normal
    float4 albedo;
    float3 specularAmount;        // Treated as a multiplier on albedo
};

SurfaceData ComputeSurfaceDataFromGeometry(VS_OUTPUT input)
{
    SurfaceData surface;
    surface.positionWorld = input.PosWorld;
    surface.normal = normalize(input.Normal);
	surface.viewDirWorld = input.ViewDir;
	
	#ifdef USE_MAP
		surface.albedo = tex2D( map, input.Texcoord0 );
		#ifdef GAMMA_INPUT
			surface.albedo.rgb *= surface.albedo.rgb;
		#endif
	#else
		surface.albedo = float4(1.0,1.0,1.0,1.0);
	#endif
	
	#ifdef USE_COLOR
		surface.albedo = surface.albedo * float4( n.Color, 1.0 );
	#endif	
	
	#ifdef ALPHATEST
		if ( surface.albedo.a * g_opacity < ALPHATEST ) discard;
	#endif
	
	#ifdef USE_SPECULARMAP
		surface.specularAmount = tex2D( specularMap, In.Texcoord0 ).rgb * g_specularColor.rgb;
	#else
		surface.specularAmount = g_specularColor.rgb;
	#endif

    return surface;
}

void AccumulatePhong(float3 normal,
                         float3 lightDir,
                         float3 viewDir,
                         float3 lightContrib,
                         inout float3 litDiffuse,
                         inout float3 litSpecular)
{
    float NdotL = dot(normal, lightDir);
    [flatten] if (NdotL > 0.0f) {
        float3 r = reflect(lightDir, normal);
        float RdotV = max(0.0f, dot(r, viewDir));
        float specular = pow(RdotV, g_shininess);

        litDiffuse += lightContrib * NdotL;
        litSpecular += lightContrib * specular;
    }
}

void AccumulateDirLight(SurfaceData surface, float3 dir, float4 color,
                    inout float3 lit)
{
    float3 litDiffuse = float3(0.0f, 0.0f, 0.0f);
    float3 litSpecular = float3(0.0f, 0.0f, 0.0f);
	
    AccumulatePhong(surface.normal, normalize(dir), normalize(surface.viewDirWorld),
        color.rgb * color.a, litDiffuse, litSpecular);
	
    lit += surface.albedo.rgb * litDiffuse + surface.specularAmount * litSpecular;
}

void AccumulatePointLight(SurfaceData surface, float3 position, float attenBegin,  float attenEnd, float4 color,
                    inout float3 lit)
{
	float3 directionToLight = position - surface.positionWorld;
    float distanceToLight = length(directionToLight);
	
    float3 litDiffuse = float3(0.0f, 0.0f, 0.0f);
    float3 litSpecular = float3(0.0f, 0.0f, 0.0f);
	
	[branch] if (distanceToLight < attenEnd) {	
		float attenuation = linstep(attenEnd, attenBegin, distanceToLight);
        directionToLight *= rcp(distanceToLight);
		
		AccumulatePhong(surface.normal, directionToLight, normalize(surface.viewDirWorld),
			attenuation * color.rgb * color.a, litDiffuse, litSpecular);
		
		lit += surface.albedo.rgb * litDiffuse + surface.specularAmount * litSpecular;
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

#ifdef GAMMA_OUTPUT
	Out.Color.rgb = sqrt( Out.Color.xyz );
#endif
	
	return Out;
}

