
uniform sampler2D image;
uniform float grayRate;

void main()
{
	vec3 origin = texture2D(image, gl_TexCoord[0].xy).rgb;
	vec3 greyscale = dot(origin, vec3(0.3, 0.59, 0.11));
	vec3 final = mix(origin, greyscale, grayRate);

    gl_FragColor = vec4(final, 1.0);
}
