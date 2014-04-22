
attribute vec3  Position;
attribute vec4  Color;
attribute vec2  Texcoord0;

varying vec4 vColor;
varying vec2 vTexCoord0;

uniform mat4 g_projMatrix;

void main()
{
	gl_Position = g_projMatrix * vec4(Position,1.0);
	vColor = Color;
	vTexCoord0.xy = Texcoord0;
}