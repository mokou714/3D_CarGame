#include "Structures.hlsli"

struct PixelShaderInput
{
	float4 pos_H : SV_POSITION;
	float3 pos_W : POSITION;
	float4 color : COLOR;
	float3 normal_world : NORMAL;
};

float4 main(PixelShaderInput input) : SV_TARGET
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
	float diffuseFactor = saturate(dot(input.normal_world, _lightDir));
	
	[flatten]
	if (diffuseFactor > 0.0f) {
		_diffuse = diffuseFactor * material.diffuse * dirLight.diffuse;
		// R = 2 * (N.L) * N - L
		float3 R = normalize(2 * saturate(dot(input.normal_world, _lightDir)) * input.normal_world - _lightDir);
		// S = (R.V)^n
		float specularFactor = pow(dot(R, _viewDir), material.specular.w);
		_specular = specularFactor * material.specular * dirLight.specular;
	}
	// I = A + D * N.L + (R.V)^n
	float4 I = input.color + (_ambient + _diffuse) + _specular;
	I.a = material.diffuse.a * input.color.a;
	return I;


	////tutorial
	//float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	//float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	//float4 spec = float4(0.0f, 0.0f, 0.0f, 0.0f);
	//float3 toEye = normalize(eyePos - input.pos_W); //V

	//// �����������䷽���෴
	//float3 lightVec = dirLight.direction;

	//// ��ӻ�����
	//ambient = material.ambient * dirLight.ambient;

	//// ����������;����
	//float diffuseFactor = dot(lightVec, input.normal_world);

	//// չ�������⶯̬��֧
	//
	//float3 v = reflect(-lightVec, input.normal_world);
	//float specFactor = pow(max(dot(v, toEye), 0.0f), material.specular.w);

	//diffuse = diffuseFactor * material.diffuse * dirLight.diffuse;
	//spec = specFactor * material.specular * dirLight.specular;

	//float4 litColor = input.color * (ambient + diffuse) + spec;
	//litColor.a = material.diffuse.a * input.color.a;
	//return litColor;
}