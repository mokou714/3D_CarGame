#include "Structures.hlsli"

struct TexPixelInput
{
	float4 pos_H : SV_POSITION;
	float3 pos_W : POSITION;
	float3 normal_world : NORMAL;
	float2 tex : TEXCOORD;
};

Texture2D Texture : register(t0);
SamplerState SamLinear : register(s0);

float4 main(TexPixelInput input) : SV_TARGET
{
	//init varibles
	float4 _ambient = {0.0f, 0.0f, 0.0f, 0.0f};
	float4 _diffuse = { 0.0f, 0.0f, 0.0f, 0.0f};
	float4 _specular = { 0.0f, 0.0f, 0.0f, 0.0f};
	float3 _lightDir = normalize(dirLight.direction); //L
	float3 _viewDir = normalize(eyePos - input.pos_W); //V

	//calculate directional light here
	// A = A
	_ambient = material.ambient * dirLight.ambient;
	// D = (N.L)
	float diffuseFactor = dot(input.normal_world, _lightDir);

	if (diffuseFactor > 0.0f) {
		_diffuse = diffuseFactor * material.diffuse * dirLight.diffuse;
		// R = 2 * (N.L) * N - L
		float3 R = normalize(2 * dot(input.normal_world, _lightDir) * input.normal_world - _lightDir);
		// Spec = (R.V)^n
		float specularFactor = pow(dot(R, _viewDir), material.specular.w);
		_specular = specularFactor * material.specular * dirLight.specular;
	}

	float4 texColor = Texture.Sample(SamLinear, input.tex);
	// I = A + D * N.L + (R.V)^n
	float4 I = texColor * (_ambient + _diffuse) + _specular;
	I.a = material.diffuse.a * texColor.a;

	return I;
}