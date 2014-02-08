
uniform sampler2D image;
uniform sampler2D imageBloom;

void main()
{
    gl_FragColor = vec4(0.0);
	gl_FragColor += texture2D(image, gl_TexCoord[0].xy);
	
	gl_FragColor += texture2D(imageBloom, gl_TexCoord[0].xy * 0.25);
}
