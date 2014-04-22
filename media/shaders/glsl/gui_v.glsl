
attribute vec3  Position;
attribute vec4  Color;
attribute vec2  Texcoord0;

varying vec4 vColor;
varying vec2 vTexCoord0;

void main()
{
	gl_Position = vec4(Position.xyz,1.0);
	vColor = Color;
	vTexCoord0.xy = Texcoord0;
}