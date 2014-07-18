
float calcSimpleShadow(sampler2D shadowMap, vec4 shadowMapPos)
{
	vec3 uv = shadowMapPos.xyz / shadowMapPos.w;
	float depth = texture2D(shadowMap, uv.xy).r;
	if(depth < uv.z - 0.1)
	{
		return 0.2;
	}
	else
	{
		return 1.0;
	}
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