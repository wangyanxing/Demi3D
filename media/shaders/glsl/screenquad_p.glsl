
uniform sampler2D image;
varying vec2 vTexCoord0;

void main()
{
    gl_FragColor = texture2D(image, vTexCoord0.xy);
}
