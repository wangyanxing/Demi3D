
#include <globals.hlsl>

struct PS_OUTPUT
{
	float4 Color : COLOR;
};

#if USE_VERT_COLOR
#	define SampleDiffuseColor(df,uv) (tex2D(df, uv)*In.Color0)
#else
#	define SampleDiffuseColor(df,uv) (tex2D(df, uv))
#endif

void SunLighting(float3 vNormal,
			float3 viewDir,
			float ks,
		    out float3 vDiffuse,
		    out float3 vSpecular)
{
    float3 Hn = normalize(viewDir + g_lightDirection);
    float4 litV = lit(dot(g_lightDirection,vNormal),dot(Hn,vNormal),g_lightIntensity);
    vDiffuse = litV.y * g_lightColor.rgb;
    vSpecular = litV.y * litV.z * ks * g_lightColor.rgb;
}

void SunLightingSpec(float3 vNormal,
			float3 viewDir,
			float3 specColor,
			float ks,
		    out float3 vDiffuse,
		    out float3 vSpecular)
{
    float3 Hn = normalize(viewDir + g_lightDirection);
    float4 litV = lit(dot(g_lightDirection,vNormal),dot(Hn,vNormal),g_lightIntensity);
    vDiffuse = litV.y * g_lightColor.rgb;
    vSpecular = litV.y * litV.z * ks * specColor;
}

float linstep(float min, float max, float v)
{
    return saturate((v - min) / (max - min));
}

#ifdef SKINNED
#   define GET_SPACE_POS_NORMAL \
       for (int i = 0; i<4; i++){ \
           objPos.xyz += mul(In.Position, g_boneMatrices[In.BlendIndices[i]]) * In.BlendWeights[i]; \
           objNormal += (mul(float4(In.Normal, 0), g_boneMatrices[In.BlendIndices[i]]) * In.BlendWeights[i]).xyz; \
       }\
       objPos.w = 1.0; \
       objNormal = normalize(objNormal);

#   define GET_SPACE_POS \
       for (int i = 0; i<4; i++)\
           objPos.xyz += mul(In.Position, g_boneMatrices[In.BlendIndices[i]]) * In.BlendWeights[i]; \
       objPos.w = 1.0; 
#else
#   define GET_SPACE_POS_NORMAL \
       objPos = In.Position;\
       objNormal = In.Normal;

#   define GET_SPACE_POS \
    objPos = In.Position;

#endif
