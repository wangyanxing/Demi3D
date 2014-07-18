
//#define UNPACK

#ifdef UNPACK
float unpackDepth( const in vec4 rgba_depth ) {
    const vec4 bit_shift = vec4( 1.0 / ( 256.0 * 256.0 * 256.0 ),
                            1.0 / ( 256.0 * 256.0 ), 1.0 / 256.0, 1.0 );
    float depth = dot( rgba_depth, bit_shift );
    return depth;
}
#endif

float calcSimpleShadow(sampler2D shadowMap, vec4 shadowMapPos)
{
	vec3 uv = shadowMapPos.xyz / shadowMapPos.w;
    
    bvec4 inFrustumVec = bvec4 ( uv.x >= 0.0, uv.x <= 1.0, uv.y >= 0.0, uv.y <= 1.0 );
    bool inFrustum = all( inFrustumVec );
    bvec2 frustumTestVec = bvec2( inFrustum, uv.z <= 1.0 );
    bool frustumTest = all( frustumTestVec );
    
    if (frustumTest)
    {
#ifdef UNPACK
        float depth = unpackDepth(texture2D(shadowMap, uv.xy));
#else
        float depth = texture2D(shadowMap, uv.xy).r;
#endif
        
        if(depth < uv.z + 0.002)
            return 0.2;
        else
            return 1.0;
    }
    return 1.0;
}

// Simple PCF
// Number of samples in one dimension (square for total samples)
#define NUM_SHADOW_SAMPLES_1D 2.0
#define SHADOW_FILTER_SCALE 1

#define SHADOW_SAMPLES NUM_SHADOW_SAMPLES_1D*NUM_SHADOW_SAMPLES_1D

vec4 offsetSample(vec4 uv, vec2 offset, float invMapSize)
{
	return vec4(uv.xy + offset * invMapSize * uv.w, uv.z, uv.w);
}

float calcDepthShadow(sampler2D shadowMap, vec4 uv, float invShadowMapSize)
{
	// 4-sample PCF
	float shadow = 0.5;
	float offset = (NUM_SHADOW_SAMPLES_1D/2 - 0.5) * SHADOW_FILTER_SCALE;
	for (float y = -offset; y <= offset; y += SHADOW_FILTER_SCALE)
		for (float x = -offset; x <= offset; x += SHADOW_FILTER_SCALE)
		{
			float depth = texture2DProj(shadowMap, offsetSample(uv, vec2(x, y), invShadowMapSize)).x;
			if (depth >= 1 || depth < uv.z + 0.00001)
				shadow += 1.0;
		}
    
	shadow /= SHADOW_SAMPLES;
    
	return shadow;
}