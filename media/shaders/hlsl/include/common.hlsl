
#include <globals.hlsl>

struct PS_OUTPUT
{
	float4 Color : COLOR;
};

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
       objNormal = normalize(objNormal);\

#   define GET_SPACE_POS_NORMAL_TANGENT \
    for (int i = 0; i<4; i++){ \
        objPos.xyz += mul(In.Position, g_boneMatrices[In.BlendIndices[i]]) * In.BlendWeights[i]; \
        objNormal += (mul(float4(In.Normal, 0), g_boneMatrices[In.BlendIndices[i]]) * In.BlendWeights[i]).xyz; \
        objTangent += (mul(float4(In.Tangent.xyz, 0), g_boneMatrices[In.BlendIndices[i]]) * In.BlendWeights[i]).xyz; \
    }\
    objPos.w = 1.0; \
    objNormal = normalize(objNormal); \
    objTangent = normalize(objTangent);


#   define GET_SPACE_POS \
       for (int i = 0; i<4; i++)\
           objPos.xyz += mul(In.Position, g_boneMatrices[In.BlendIndices[i]]) * In.BlendWeights[i]; \
       objPos.w = 1.0; 
#else
#   define GET_SPACE_POS_NORMAL \
       objPos = In.Position;\
       objNormal = In.Normal;

#   define GET_SPACE_POS_NORMAL_TANGENT \
       objPos = In.Position; \
       objNormal = In.Normal; \
       objTangent = In.Tangent.xyz;

#   define GET_SPACE_POS \
    objPos = In.Position;

#endif
