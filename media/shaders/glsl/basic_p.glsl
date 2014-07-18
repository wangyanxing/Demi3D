#include "common.h"
#include "shadows.h"

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

#if defined( SHADOW_RECEIVER )
varying vec4 vLightSpacePos;
uniform float invShadowMapSize;
uniform sampler2D shadowMap;
#endif

void main()
{
	gl_FragColor = vec4( g_diffuseColor, g_opacity );

	float fShadow = 1.0;
#if defined( SHADOW_RECEIVER )
	//fShadow = calcDepthShadow( shadowMap, vLightSpacePos, invShadowMapSize );
	fShadow = calcSimpleShadow( shadowMap, vLightSpacePos );
#endif
	
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
	gl_FragColor = gl_FragColor * vColor;
#endif	

	gl_FragColor.rgb *= fShadow;

#ifdef GAMMA_OUTPUT
	gl_FragColor = sqrt( gl_FragColor.xyz );
#endif
}
