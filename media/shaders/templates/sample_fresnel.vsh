
#include <globals.hlsl>

uniform float mRefractionRatio;
uniform float mFresnelBias;
uniform float mFresnelScale;
uniform float mFresnelPower;

struct VS_INPUT
{
	float4 Position   	: POSITION;
	float3 Normal		: NORMAL;
};

struct VS_OUTPUT
{
	float4 Position 	: POSITION;

	float3 Reflect      : TEXCOORD1;
	float3 Refract0 	: TEXCOORD2;
	float3 Refract1 	: TEXCOORD3;
	float3 Refract2 	: TEXCOORD4;
	float  ReflectFactor: TEXCOORD5;
};

VS_OUTPUT vs_main( VS_INPUT In )
{
	VS_OUTPUT Out;

	Out.Position = mul(g_modelViewProjMatrix,In.Position);
  
	float4x4 model;
	float4x4 m			= g_modelMatrix;
	model[0]			= float4(m[0].xyz,0.0);
	model[1]			= float4(m[1].xyz,0.0);
	model[2]			= float4(m[2].xyz,0.0);
	model[3]			= float4(0,0,0,1);
    float3 normal       = mul(model, float4(normalize(In.Normal), 1.0f)).xyz;
	normal = normalize(normal);
	float3 posWorld		= mul(g_modelMatrix,In.Position);
	float3 viewDir 		= posWorld - g_eyePosition;
	
	Out.Reflect  = reflect( viewDir, normal );
	Out.Refract0 = refract( normalize(viewDir), normal, mRefractionRatio );
	Out.Refract1 = refract( normalize(viewDir), normal, mRefractionRatio * 0.99 );
	Out.Refract2 = refract( normalize(viewDir), normal, mRefractionRatio * 0.98 );
	Out.ReflectFactor = mFresnelBias + mFresnelScale * pow( 1.0 + dot( normalize( viewDir ), normal ), mFresnelPower );
	
	return Out;
}


