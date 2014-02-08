
uniform samplerCube cubeMap;

varying vec3 vReflect;
varying vec3 vRefract[3];
varying float vReflectionFactor;

void main() 
{
	vec4 reflectedColor = textureCube( cubeMap, vReflect );
	vec4 refractedColor = vec4( 1.0,1.0,1.0,1.0 );

	refractedColor.r = textureCube( cubeMap, vRefract[0]).r;
	refractedColor.g = textureCube( cubeMap, vRefract[1]).g;
	refractedColor.b = textureCube( cubeMap, vRefract[2]).b;

	gl_FragColor = mix( refractedColor, reflectedColor, clamp( vReflectionFactor, 0.0, 1.0 ) );

}