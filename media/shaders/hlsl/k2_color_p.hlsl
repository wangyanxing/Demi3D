
#include <common.hlsl>

// options
//define USE_SCHLICK_FRESNEL

#ifdef USE_MAP
	uniform sampler2D map;
#endif

#ifdef USE_NORMALMAP
    uniform sampler2D normalMap;
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

static SurfaceData gSurface;

void ComputeSurfaceDataFromGeometry(VS_OUTPUT input)
{
    gSurface.positionWorld = input.PosWorld;
    gSurface.viewDirWorld = input.ViewDir;
    gSurface.specularAmount = float3(1, 1, 1);

#if defined( USE_NORMALMAP )
    float4 normMapColor;
    float4 nmTex = tex2D(normalMap, input.Texcoord0);
    normMapColor.rgb = nmTex.agb;
    gSurface.specularAmount *= tex2D(specularMap, input.Texcoord0).g;
    float3 texNormal = float3(normMapColor.rgb * 2.0f - 1.0f);
    float3x3 rot = float3x3(input.Tangent, input.Binormal, input.Normal);
    gSurface.normal = normalize(mul(texNormal, rot));
#else
    gSurface.normal = normalize(input.Normal);
#endif
	
#ifdef USE_MAP
    gSurface.albedo = tex2D(map, input.Texcoord0);
	#ifdef GAMMA_INPUT
    gSurface.albedo.rgb *= gSurface.albedo.rgb;
	#endif
#else
    gSurface.albedo = float4(1.0, 1.0, 1.0, 1.0);
#endif

#ifdef USE_COLOR
    gSurface.albedo = gSurface.albedo * float4(n.Color, 1.0);
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
	
    for (int i = 0; i < g_numDirLights; i++){
        AccumulateDirLight(g_dirLightsDir[i], g_dirLightsColor[i],
            vDiffuse, vSpecular);
    }

	#if 0
    for (int i = 0; i < g_numPointLights; i++){
        AccumulatePointLight(g_pointLightsPosition[i],
            g_pointLightsAttenuation[i].x, g_pointLightsAttenuation[i].y,
            g_pointLightsColor[i], vDiffuse, vSpecular);
    }
	#endif

    float3 vFinalColor = gSurface.albedo * vDiffuse + vSpecular;

    Out.Color.rgb = vFinalColor;
    Out.Color.a = gSurface.albedo.a * g_opacity;

#ifdef GAMMA_OUTPUT
    Out.Color.rgb = sqrt(Out.Color.xyz);
#endif

	return Out;
}