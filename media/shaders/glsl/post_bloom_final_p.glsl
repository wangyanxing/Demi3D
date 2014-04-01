
uniform sampler2D image;
uniform sampler2D imageBloom;

varying vec2 vTexCoord0;

void main()
{
    gl_FragColor = vec4(0.0);
	gl_FragColor += texture2D(image, vTexCoord0.xy);
	gl_FragColor += texture2D(imageBloom, vTexCoord0.xy * 0.25);
}
