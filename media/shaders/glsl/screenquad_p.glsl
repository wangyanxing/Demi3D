
uniform sampler2D image;

void main()
{
    gl_FragColor = texture2D(image, gl_TexCoord[0].xy);
}
