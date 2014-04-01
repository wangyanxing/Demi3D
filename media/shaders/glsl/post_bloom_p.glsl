
uniform sampler2D image;

varying vec2 vTexCoord0;
varying vec4 vTexCoord1;
varying vec4 vTexCoord2;
varying vec4 vTexCoord3;
varying vec4 vTexCoord4;
varying vec4 vTexCoord5;
varying vec4 vTexCoord6;

void main()
{
	gl_FragColor = vec4(0.0,0.0,0.0,0.0);
    
	gl_FragColor += texture2D(image, vTexCoord0.xy) * 0.003450;
	gl_FragColor += texture2D(image, vTexCoord1.xy) * 0.012483;
	gl_FragColor += texture2D(image, vTexCoord1.zw) * 0.035351;
	gl_FragColor += texture2D(image, vTexCoord2.xy) * 0.078353;
	gl_FragColor += texture2D(image, vTexCoord2.zw) * 0.135933;
	gl_FragColor += texture2D(image, vTexCoord3.xy) * 0.184604;
	gl_FragColor += texture2D(image, vTexCoord3.zw) * 0.099654;
	gl_FragColor += texture2D(image, vTexCoord4.xy) * 0.184604;
	gl_FragColor += texture2D(image, vTexCoord4.zw) * 0.135933;
	gl_FragColor += texture2D(image, vTexCoord5.xy) * 0.078353;
	gl_FragColor += texture2D(image, vTexCoord5.zw) * 0.035351;
	gl_FragColor += texture2D(image, vTexCoord6.xy) * 0.012483;
	gl_FragColor += texture2D(image, vTexCoord6.zw) * 0.003450;
}
