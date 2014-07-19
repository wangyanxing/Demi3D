varying vec4 vColor;
uniform sampler2D map;
varying vec2 vTexCoord0;

uniform vec3 g_diffuseColor;
uniform float g_opacity;

void main()
{
	vec4 final = vec4( g_diffuseColor, g_opacity );
	
	vec4 texelColor = texture2D( map, vTexCoord0.xy );
	final = final * texelColor;

#ifdef ALPHATEST
	if ( final.a < ALPHATEST ) discard;
#endif
	
	gl_FragColor = final * vColor;
}