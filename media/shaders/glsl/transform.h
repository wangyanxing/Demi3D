
#ifdef SKINNED
#   define GET_SPACE_POS_NORMAL \
       for (int i = 0; i<4; i++){ \
           objPos.xyz += (Position * g_boneMatrices[In.BlendIndices[i]]) * BlendWeights[i]; \
           objNormal += ((vec4(Normal, 0) * g_boneMatrices[BlendIndices[i]]) * BlendWeights[i]).xyz; \
       }\
       objPos.w = 1.0; \
       objNormal = normalize(objNormal);

#   define GET_SPACE_POS \
       for (int i = 0; i<4; i++)\
           objPos.xyz += (Position * g_boneMatrices[BlendIndices[i]]) * BlendWeights[i]; \
       objPos.w = 1.0; 
#else
#   define GET_SPACE_POS_NORMAL(p, n) \
       vec4 objPos = vec4(Position.xyz,1.0);\
       vec3 objNormal = Normal;

#   define GET_SPACE_POS(p) \
       vec4 objPos = vec4(Position.xyz,1.0);

#endif

