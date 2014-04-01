
uniform samplerCube environmentTexture;
varying vec3 vTexCoord0;

void main()
{
	gl_FragColor = textureCube(environmentTexture, vTexCoord0.xyz);
}
