
varying vec2 vDepth;

#define LINEAR_RANGE 1

void main()
{
#if LINEAR_RANGE
	float finalDepth = vDepth.x;
#else
	float finalDepth = vDepth.x / vDepth.y;
#endif
	// just smear across all components 
	// therefore this one needs high individual channel precision
	gl_FragColor = vec4(finalDepth, finalDepth, finalDepth, 1);

}