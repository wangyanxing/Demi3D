#include "common.h"

// this shader is adapted from 
// https://www.shadertoy.com/view/XsfGRn
// Created by inigo quilez - iq/2013
// License Creative Commons Attribution-NonCommercial-ShareAlike 3.0 Unported License.


void main(void)
{
	vec2 p = (2.0*gl_FragCoord.xy - g_viewportSize.xy) / min(g_viewportSize.y,g_viewportSize.x);
	
	p.y -= 0.25;

    // background color
    vec3 bcol = vec3(1.0,0.8,0.7-0.07*p.y)*(1.0-0.25*length(p));

    // animate
    float tt = mod(g_time,1.5)/1.5;
    float ss = pow(tt,.2)*0.5 + 0.5;
    ss -= ss*0.2*sin(tt*6.2831*3.0)*exp(-tt*4.0);
    p *= vec2(0.5,1.5) + ss*vec2(0.5,-0.5);
   

    // shape
    float a = atan(p.x,p.y)/3.141593;
    float r = length(p);
    float h = abs(a);
    float d = (13.0*h - 22.0*h*h + 10.0*h*h*h)/(6.0-5.0*h);

	// color
	float s = 1.0-0.5*clamp(r/d,0.0,1.0);
	s = 0.75 + 0.75*p.x;
	s *= 1.0-0.25*r;
	s = 0.5 + 0.6*s;
	s *= 0.5+0.5*pow( 1.0-clamp(r/d, 0.0, 1.0 ), 0.1 );
	vec3 hcol = vec3(1.0,0.5*r,0.3)*s;
	
    vec3 col = mix( bcol, hcol, smoothstep( -0.01, 0.01, d-r) );

    gl_FragColor = vec4(col,1.0);
}