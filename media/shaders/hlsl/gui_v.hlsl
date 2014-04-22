
struct VS_INPUT
{
	float4 Position   	: POSITION;
	float2 Texcoord0  	: TEXCOORD0;
	half4  Color		: COLOR;
};

struct VS_OUTPUT
{
	float4 Position 	: POSITION;
	half4  Color		: COLOR;
	float2 Texcoord0  	: TEXCOORD0;
};

VS_OUTPUT vs_main( VS_INPUT In )
{
    VS_OUTPUT Out;
    Out.Position  = float4(In.Position.xyz, 1.0);
	Out.Color     = In.Color;
	Out.Texcoord0 = In.Texcoord0;
	return Out;
}