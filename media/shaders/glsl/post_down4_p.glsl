
uniform sampler2D image;

void main()
{
	gl_FragColor = vec4(0.0);

	gl_FragColor += texture2D(image, gl_TexCoord[0].xy);
	gl_FragColor += texture2D(image, gl_TexCoord[1].xy);
	gl_FragColor += texture2D(image, gl_TexCoord[2].xy);
	gl_FragColor += texture2D(image, gl_TexCoord[3].xy);
	
	gl_FragColor *= 0.25f;
}
