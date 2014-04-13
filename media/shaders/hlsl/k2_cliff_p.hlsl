
#include <common.hlsl>

// options
//define USE_SCHLICK_FRESNEL

uniform sampler2D map;
uniform sampler2D terrainMap;

#ifdef USE_NORMALMAP
uniform sampler2D normalMap;
uniform sampler2D specularMap;
uniform sampler2D terrainNormalMap;
uniform sampler2D terrainSpecularMap;
#endif 

struct VS_OUTPUT
{
	float4 Position 	: POSITION;

	//half4  Color		: COLOR;

	float4 Texcoord0  	: TEXCOORD0;
	float3 Normal       : TEXCOORD1;
    float3 ViewDir	 	: TEXCOORD2;
	float3 PosWorld 	: TEXCOORD3;

#if defined( USE_NORMALMAP )
	float3 Tangent		   : TEXCOORD4;
	float3 Binormal		   : TEXCOORD5;
    float3 TerrainTangent  : TEXCOORD6;
    float3 TerrainBinormal : TEXCOORD7;
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

static SurfaceData gSurface;

void ComputeSurfaceDataFromGeometry(VS_OUTPUT input)
{
    gSurface.positionWorld = input.PosWorld;
    gSurface.viewDirWorld = input.ViewDir;

    float4 cliffDiffuse = tex2D(map, input.Texcoord0.xy);
    float4 terrainDiffuse = tex2D(terrainMap, input.Texcoord0.zw);
    gSurface.albedo.rgb = lerp(terrainDiffuse.rgb, cliffDiffuse.rgb, cliffDiffuse.a)/* * input.Color*/;
    gSurface.albedo.a = 1.0;

#if defined( USE_NORMALMAP )
    float4 cCliffNormalmapColor;
    cCliffNormalmapColor.rgb = tex2D(normalMap, input.Texcoord0.xy).agb;
    cCliffNormalmapColor.a = tex2D(specularMap, input.Texcoord0.xy).g;

    float4 cTerrainNormalmapColor;
    cTerrainNormalmapColor.rgb = tex2D(terrainNormalMap, input.Texcoord0.zw).agb;
    cTerrainNormalmapColor.a = tex2D(terrainSpecularMap, input.Texcoord0.zw).g;

    gSurface.specularAmount.rgb = lerp(cTerrainNormalmapColor.a, cCliffNormalmapColor.a, cliffDiffuse.a);

    float3 vTexNormalCliff = float3(cCliffNormalmapColor.rgb * 2.0f - 1.0f);
    float3 vTexNormalTerrain = float3(cTerrainNormalmapColor.rgb * 2.0f - 1.0f);

    float3x3 rotCliff = float3x3(input.Tangent, input.Binormal, input.Normal);
    float3x3 rotTerrain = float3x3(input.TerrainTangent, input.TerrainBinormal, input.Normal);
    float3 vNormalTerrain = mul(vTexNormalTerrain, rotTerrain);
    float3 vNormalCliff = mul(vTexNormalCliff, rotCliff);
    gSurface.normal = normalize(lerp(vNormalTerrain, vNormalCliff, cliffDiffuse.a));
#else
    gSurface.normal = normalize(input.Normal);
#endif

	#ifdef GAMMA_INPUT
    gSurface.albedo.rgb *= gSurface.albedo.rgb;
	#endif
}

void AccumulatePhong(float3 normal,
                         float3 lightDir,
                         float3 viewDir,
                         float3 lightContrib,
                         inout float3 litDiffuse,
                         inout float3 litSpecular)
{
    float NdotL = max(dot(normal, lightDir), 0.0);
    float3 halfVec = normalize(lightDir + viewDir);
    float dotHalf = max(dot(normal, halfVec), 0.0);
    float specWeight = max(pow(dotHalf, g_shininess), 0.0);

#ifdef USE_SCHLICK_FRESNEL
    float specularNormalization = (g_shininess + 2.0001) / 8.0;
    float3 schlick = gSurface.specularAmount + float3(1.0 - gSurface.specularAmount)
        * pow(1.0 - dot(lightDir, halfVec), 5.0);

    float3 specular = schlick * specWeight * NdotL * specularNormalization;
#else
    float3 specular = gSurface.specularAmount * specWeight;
#endif

    litDiffuse += lightContrib * NdotL;
    litSpecular += lightContrib * specular;
}

void AccumulateDirLight(float3 dir, float4 color, 
    inout float3 diffuse, inout float3 specular) {

    float3 litDiffuse = float3(0.0f, 0.0f, 0.0f);
    float3 litSpecular = float3(0.0f, 0.0f, 0.0f);
	
    AccumulatePhong(gSurface.normal, normalize(-dir), normalize(gSurface.viewDirWorld),
        color.rgb * color.a, litDiffuse, litSpecular);
	
    diffuse += litDiffuse;
    specular += litSpecular;
}

void AccumulatePointLight(float3 position, float attenBegin, float attenEnd, float4 color, 
    inout float3 diffuse, inout float3 specular) {

    float3 directionToLight = position - gSurface.positionWorld;
    float distanceToLight = length(directionToLight);
	
    float3 litDiffuse = float3(0.0f, 0.0f, 0.0f);
    float3 litSpecular = float3(0.0f, 0.0f, 0.0f);
	
	[branch] if (distanceToLight < attenEnd) {	
		float attenuation = linstep(attenEnd, attenBegin, distanceToLight);
        directionToLight *= rcp(distanceToLight);
		
        AccumulatePhong(gSurface.normal, directionToLight, normalize(gSurface.viewDirWorld),
			attenuation * color.rgb * color.a, litDiffuse, litSpecular);
		
        diffuse += litDiffuse;
        specular += litSpecular;
	}
}


PS_OUTPUT ps_main( VS_OUTPUT In )
{			
	PS_OUTPUT Out;
	
	ComputeSurfaceDataFromGeometry(In);
	
#ifdef ALPHATEST
    if (gSurface.albedo.a * g_opacity < ALPHATEST) discard;
#endif

    float3 vDiffuse = g_globalAmbient.rgb;
    float3 vSpecular = float3(0.0f, 0.0f, 0.0f);
	
	for(int i = 0; i < g_numDirLights; i++){
        AccumulateDirLight(g_dirLightsDir[i], g_dirLightsColor[i], 
            vDiffuse, vSpecular);
	}
	
#if 0
	for(int i = 0; i < g_numPointLights; i++){
		AccumulatePointLight(g_pointLightsPosition[i], 
            g_pointLightsAttenuation[i].x, g_pointLightsAttenuation[i].y,
            g_pointLightsColor[i], vDiffuse, vSpecular);
	}
#endif

    float3 vFinalColor = gSurface.albedo * vDiffuse + vSpecular;
	
    Out.Color.rgb = vFinalColor;
    Out.Color.a = gSurface.albedo.a * g_opacity;

#ifdef GAMMA_OUTPUT
	Out.Color.rgb = sqrt( Out.Color.xyz );
#endif

	return Out;
}