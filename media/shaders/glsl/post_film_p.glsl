#include "common.h"

uniform sampler2D image;

uniform float nIntensity;
uniform float sIntensity;
uniform float sCount;

void main()
{
	vec4 origin = texture2D(image, gl_TexCoord[0].xy);
	
	// make some noise
	float x = gl_TexCoord[0].x * gl_TexCoord[0].y * g_time *  1000.0;
	x = mod( x, 13.0 ) * mod( x, 123.0 );
	float dx = mod( x, 0.01 );
	
	// apply noise
	vec3 cResult = origin.rgb + origin.rgb * clamp( 0.1 + dx * 100.0, 0.0, 1.0 );
	
	vec2 sc = vec2( sin( gl_TexCoord[0].y * sCount ), cos( gl_TexCoord[0].y * sCount ) );
	
	cResult += origin.rgb * vec3( sc.x, sc.y, sc.x ) * sIntensity;
	cResult = origin.rgb + clamp( nIntensity, 0.0,1.0 ) * ( cResult - origin.rgb );
	
    gl_FragColor =  vec4( cResult, origin.a );
}
