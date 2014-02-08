
uniform sampler2D image;

void main()
{
	gl_FragColor = vec4(0.0);
	
    
	gl_FragColor += texture2D(image, gl_TexCoord[0].xy) * 0.003450;
	gl_FragColor += texture2D(image, gl_TexCoord[1].xy) * 0.012483;
	gl_FragColor += texture2D(image, gl_TexCoord[1].zw) * 0.035351;
	gl_FragColor += texture2D(image, gl_TexCoord[2].xy) * 0.078353;
	gl_FragColor += texture2D(image, gl_TexCoord[2].zw) * 0.135933;
	gl_FragColor += texture2D(image, gl_TexCoord[3].xy) * 0.184604;
	gl_FragColor += texture2D(image, gl_TexCoord[3].zw) * 0.099654;
	gl_FragColor += texture2D(image, gl_TexCoord[4].xy) * 0.184604;
	gl_FragColor += texture2D(image, gl_TexCoord[4].zw) * 0.135933;
	gl_FragColor += texture2D(image, gl_TexCoord[5].xy) * 0.078353;
	gl_FragColor += texture2D(image, gl_TexCoord[5].zw) * 0.035351;
	gl_FragColor += texture2D(image, gl_TexCoord[6].xy) * 0.012483;
	gl_FragColor += texture2D(image, gl_TexCoord[6].zw) * 0.003450;
     
}
