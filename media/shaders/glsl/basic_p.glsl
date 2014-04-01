#include "common.h"

#if defined( USE_COLOR )
varying vec4 vColor;
#endif

#ifdef USE_MAP
uniform sampler2D map;
varying vec2 vTexCoord0;
#endif

#ifdef USE_LIGHTMAP
uniform sampler2D lightMap;
varying vec2 vTexCoord1;
#endif

void main()
{
	gl_FragColor = vec4( g_diffuseColor, g_opacity );
	
#ifdef USE_MAP
	vec4 texelColor = texture2D( map, vTexCoord0.xy );
	#ifdef GAMMA_INPUT
		texelColor.xyz *= texelColor.xyz;
	#endif
	gl_FragColor = gl_FragColor * texelColor;
#endif

#ifdef ALPHATEST
	if ( gl_FragColor.a < ALPHATEST ) discard;
#endif

#ifdef USE_LIGHTMAP
	gl_FragColor = gl_FragColor * texture2D( lightMap, vTexCoord1.xy );
#endif
	
#ifdef USE_COLOR
	gl_FragColor = gl_FragColor * vec4( vColor.xyz, g_opacity );
#endif	

#ifdef GAMMA_OUTPUT
	gl_FragColor = sqrt( gl_FragColor.xyz );
#endif
}
