
uniform sampler2D image;
uniform float grayRate;
varying vec2 vTexCoord0;

void main()
{
	vec3 origin = texture2D(image, vTexCoord0.xy).rgb;
	vec3 greyscale = dot(origin, vec3(0.3, 0.59, 0.11));
	vec3 final = mix(origin, greyscale, grayRate);

    gl_FragColor = vec4(final, 1.0);
}
