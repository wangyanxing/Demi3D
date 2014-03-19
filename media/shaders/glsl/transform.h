
#ifdef SKINNED

#   define GET_SPACE_POS_NORMAL(p, n) \
	   mat3x4 _blend = mat3x4(0.0);   \
	   for (int i = 0; i < 4; i++)    \
	       _blend += g_boneMatrices[int(BlendIndices[i])] * BlendWeights[i]; \
	   vec4 objPos = vec4((vec4(Position,1.0) * _blend).xyz, 1.0); \
	   vec3 objNormal = normalize((vec4(Normal, 0.0) * _blend).xyz);

#   define GET_SPACE_POS_NORMAL_TANGENT(p, n, t) \
        mat3x4 _blend = mat3x4(0.0);   \
        for (int i = 0; i < 4; i++)    \
            _blend += g_boneMatrices[int(BlendIndices[i])] * BlendWeights[i]; \
        vec4 objPos = vec4((vec4(Position,1.0) * _blend).xyz, 1.0); \
        vec3 objNormal = normalize((vec4(Normal, 0.0) * _blend).xyz);\
        vec3 objTangent = normalize((vec4(Tangent, 0.0) * _blend).xyz);\

#   define GET_SPACE_POS(p) \
	   vec4 objPos = vec4(0.0);\
       for (int i = 0; i<1; i++)\
           objPos.xyz += (vec4(Position,1.0)*g_boneMatrices[int(BlendIndices[i])]).xyz; \
       objPos.w = 1.0; 
#else

#   define GET_SPACE_POS_NORMAL(p, n) \
       vec4 objPos = vec4(Position.xyz,1.0);\
       vec3 objNormal = Normal;

#   define GET_SPACE_POS_NORMAL_TANGENT(p, n, t) \
        vec4 objPos = vec4(Position.xyz,1.0);\
        vec3 objNormal = Normal; \
        vec3 objTangent = Tangent;

#   define GET_SPACE_POS(p) \
       vec4 objPos = vec4(Position.xyz,1.0);

#endif

