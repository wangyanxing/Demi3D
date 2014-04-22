
uniform sampler2D map;
varying vec4 vColor;
varying vec2 vTexCoord0;

void main()
{
	gl_FragColor = texture2D( map, vTexCoord0.xy ) * vColor;
}
