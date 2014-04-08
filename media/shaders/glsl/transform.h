
#ifdef SKINNED

#ifdef DI_GLES2
#   define GET_SPACE_POS_NORMAL(p, n) \
        mat4 _blend = mat4(0.0);   \
        _blend[3][3] = 1.0; \
        for (int i = 0; i < 4; i++) {  \
            _blend[0] += g_boneMatrices[int(BlendIndices[i])*3 + 0] * BlendWeights[i]; \
            _blend[1] += g_boneMatrices[int(BlendIndices[i])*3 + 1] * BlendWeights[i]; \
            _blend[2] += g_boneMatrices[int(BlendIndices[i])*3 + 2] * BlendWeights[i]; \
        } \
        vec4 objPos = vec4((vec4(Position,1.0) * _blend).xyz, 1.0); \
        vec3 objNormal = normalize((vec4(Normal.xyz, 0.0) * _blend).xyz);

#   define GET_SPACE_POS_NORMAL_TANGENT(p, n, t) \
        mat4 _blend = mat4(0.0);   \
        _blend[3][3] = 1.0; \
        for (int i = 0; i < 4; i++) {  \
            _blend[0] += g_boneMatrices[int(BlendIndices[i])*3 + 0] * BlendWeights[i]; \
            _blend[1] += g_boneMatrices[int(BlendIndices[i])*3 + 1] * BlendWeights[i]; \
            _blend[2] += g_boneMatrices[int(BlendIndices[i])*3 + 2] * BlendWeights[i]; \
        } \
        vec4 objPos = vec4((vec4(Position,1.0) * _blend).xyz, 1.0); \
        vec3 objNormal = normalize((vec4(Normal.xyz, 0.0) * _blend).xyz);\
        vec3 objTangent = normalize((vec4(Tangent.xyz, 0.0) * _blend).xyz);

#else

#   define GET_SPACE_POS_NORMAL(p, n) \
	   mat3x4 _blend = mat3x4(0.0);   \
	   for (int i = 0; i < 4; i++)    \
	       _blend += g_boneMatrices[int(BlendIndices[i])] * BlendWeights[i]; \
	   vec4 objPos = vec4((vec4(Position,1.0) * _blend).xyz, 1.0); \
	   vec3 objNormal = normalize((vec4(Normal.xyz, 0.0) * _blend).xyz);

#   define GET_SPACE_POS_NORMAL_TANGENT(p, n, t) \
        mat3x4 _blend = mat3x4(0.0);   \
        for (int i = 0; i < 4; i++)    \
            _blend += g_boneMatrices[int(BlendIndices[i])] * BlendWeights[i]; \
        vec4 objPos = vec4((vec4(Position,1.0) * _blend).xyz, 1.0); \
        vec3 objNormal = normalize((vec4(Normal.xyz, 0.0) * _blend).xyz);\
        vec3 objTangent = normalize((vec4(Tangent.xyz, 0.0) * _blend).xyz);\

#endif

#   define GET_SPACE_POS(p) \
	   vec4 objPos = vec4(0.0);\
       for (int i = 0; i < 1; i++)\
           objPos.xyz += (vec4(Position,1.0)*g_boneMatrices[int(BlendIndices[i])]).xyz; \
       objPos.w = 1.0; 
#else

#   define GET_SPACE_POS_NORMAL(p, n) \
       vec4 objPos = vec4(Position.xyz,1.0);\
       vec3 objNormal = Normal.xyz;

#   define GET_SPACE_POS_NORMAL_TANGENT(p, n, t) \
        vec4 objPos = vec4(Position.xyz,1.0);\
        vec3 objNormal = Normal.xyz; \
        vec3 objTangent = Tangent.xyz;

#   define GET_SPACE_POS(p) \
       vec4 objPos = vec4(Position.xyz,1.0);

#endif

