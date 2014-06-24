#include "common.h"

varying vec4 vColor;
uniform sampler2D map;
varying vec2 vTexCoord0;

void main()
{
	gl_FragColor = vec4( g_diffuseColor, g_opacity );
	
	vec4 texelColor = texture2D( map, vTexCoord0.xy );
	gl_FragColor = gl_FragColor * texelColor;

#ifdef ALPHATEST
	if ( gl_FragColor.a < ALPHATEST ) discard;
#endif
	
	gl_FragColor.rgb = gl_FragColor.rgb * vColor.rgb;
    gl_FragColor.a = vColor.a;
}
