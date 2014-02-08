
uniform samplerCube environmentTexture;

void main()
{
	gl_FragColor = textureCube(environmentTexture, gl_TexCoord[0].xyz);
}
