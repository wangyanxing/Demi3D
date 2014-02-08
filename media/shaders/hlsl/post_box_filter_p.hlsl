
#include <common.hlsl>

// a very simple 4x4 box filter
// the kernel has the following form
//   o o o o
//   o o o o
//   o o x o
//   o o o o 
// where x marks the fragment position and the o marks a sampling point

uniform sampler image;

void ps_main
(
    in float2 screenTC : TEXCOORD0,    
    out float4 oColor0 : COLOR0
)
{
    float color = 0;
    for (int x = -2; x < 2; x++)
    for (int y = -2; y < 2; y++)
    {
        color += tex2D(image, float2(screenTC.x + x * g_viewportSize.z, screenTC.y + y * g_viewportSize.w)).x;
    }
    color /= 16;
        
    oColor0 = float4(color.xxx, 1);
}
