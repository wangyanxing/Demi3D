
#include "common.h"

uniform sampler2D image;

uniform vec3 vLinearBright;

void main()
{
	gl_FragColor = texture2D(image, gl_TexCoord[0].xy);
	
	float fBrightness = max(gl_FragColor.r, max(gl_FragColor.g, gl_FragColor.b)) + dot(gl_FragColor.rgb, vec3(1.0, 1.0, 1.0));
	float fScale = clamp(fBrightness * vLinearBright.x + vLinearBright.y, 0.0, 1.0) * vLinearBright.z;

	gl_FragColor.rgb *= fScale;
}


// hdrParam.x: hdrb bright Thres
// hdrParam.y: hdrb bright offset
/*
uniform vec2 hdrParam;

vec4 getTex2DBilinear( const sampler2D tex, const vec2 texCoord, const vec2 texSize )
{
	// Bilinear filtering function. Useful when hardware filtering is not available, e.g. for
	// floating point textures on ATI 1xx0 cards
	
	vec2 coord0 = texCoord - 0.5 / texSize;
	vec2 coord1 = texCoord + 0.5 / texSize;
	vec2 weight = fract( coord0 * texSize );
	
	vec4 bot = mix( texture2D( tex, coord0 ),
					texture2D( tex, vec2( coord1.x, coord0.y ) ),
					weight.x );
	vec4 top = mix( texture2D( tex, vec2( coord0.x, coord1.y ) ),
					texture2D( tex, coord1 ),
					weight.x );
	
	return mix( bot, top, weight.y );
}


void main( void )
{
	vec2 texSize = g_viewportSize.xy * 4.0;
	vec2 coord2 = gl_TexCoord[0].xy + vec2( 2.0, 2.0 ) / texSize;
	
	// Average using bilinear filtering
	vec4 sum = getTex2DBilinear( image, gl_TexCoord[0].xy, texSize );
	sum += getTex2DBilinear( image, coord2, texSize );
	sum += getTex2DBilinear( image, vec2( coord2.x, gl_TexCoord[0].y ), texSize );
	sum += getTex2DBilinear( image, vec2( gl_TexCoord[0].x, coord2.y ), texSize );
	sum /= 4.0;
	
	// Tonemap
	//sum = 1.0 - exp2( -hdrExposure * sum );
	
	// Extract bright values
	sum = max( sum - hdrParam.x, 0.0 );
	sum /= hdrParam.y + sum;
	
	gl_FragColor = sum;
}

*/
