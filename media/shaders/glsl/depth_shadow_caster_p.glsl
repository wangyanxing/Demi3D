
varying vec2 vDepth;

#define LINEAR_RANGE 1

vec4 pack_depth( const in float depth ) {
    const vec4 bit_shift = vec4( 256.0 * 256.0 * 256.0, 256.0 * 256.0, 256.0, 1.0 );
    const vec4 bit_mask  = vec4( 0.0, 1.0 / 256.0, 1.0 / 256.0, 1.0 / 256.0 );
    vec4 res = fract( depth * bit_shift );
    res -= res.xxyz * bit_mask;
    return res;
}

void main()
{
#if LINEAR_RANGE
	float finalDepth = vDepth.x;
#else
	float finalDepth = vDepth.x / vDepth.y;
#endif
	// just smear across all components 
	// therefore this one needs high individual channel precision
	//gl_FragColor = vec4(finalDepth, finalDepth, finalDepth, 1);

#ifdef UNPACK
    gl_FragData[ 0 ] = pack_depth( gl_FragCoord.z );
#else
    gl_FragData[ 0 ] = vec4(gl_FragCoord.z, gl_FragCoord.z, gl_FragCoord.z, gl_FragCoord.z);
#endif
}