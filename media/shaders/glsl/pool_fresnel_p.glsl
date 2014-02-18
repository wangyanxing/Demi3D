
uniform vec4 tintColour;
uniform float noiseScale;
uniform float fresnelBias;
uniform float fresnelScale;
uniform float fresnelPower;

uniform sampler2D noiseMap;
uniform sampler2D reflectMap;
uniform sampler2D refractMap;

varying vec4 projectionCoord;
varying vec2 noiseCoord;
varying vec3 vNormal;
varying vec3 vViewDir;

// Fragment program for distorting a texture using a 3D noise texture
void main()
{
	// Do the tex projection manually so we can distort _after_
	vec2 final = projectionCoord.xy / projectionCoord.w;

	
	// Noise
	vec3 noiseNormal = (texture2D(noiseMap, (noiseCoord.xy / 5.0)).rgb - 0.5).rbg * noiseScale;
	final += noiseNormal.xz;
	
	// Fresnel
	float fresnel = fresnelBias + fresnelScale * pow(1.0 + 
		dot(normalize(vViewDir), normalize(vNormal)), fresnelPower);

	// Reflection / refraction
	vec4 reflectionColour = texture2D(reflectMap, final);
	vec4 refractionColour = texture2D(refractMap, final) + tintColour;

	// Final colour
	gl_FragColor = mix(refractionColour, reflectionColour, fresnel);
	
	
	//gl_FragColor = texture2D(reflectMap,  final);
}
