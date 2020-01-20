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
	////init varibles
	//float4 _ambient = {0.0f, 0.0f, 0.0f, 0.0f};
	//float4 _diffuse = {0.0f, 0.0f, 0.0f, 0.0f};
	//float4 _specular = {0.0f, 0.0f, 0.0f, 0.0f};
	//float3 lightDir = normalize(dirLight.direction); //L
	//float3 toEyeDir = normalize(eyePos - input.pos_W); //V

	////calculate directional light here
	//// R = 2 * (N.L) * N - L
	//float3 R = normalize(2 * dot(input.normal_world, lightDir) * input.normal_world - lightDir);
	//// S = (R.V)^n
	//float specularFactor = pow(saturate(dot(R, toEyeDir)), material.specular.w);
	//_specular = specularFactor * material.specular * dirLight.specular;
	//// D = (N.L)
	//float diffuseFactor = saturate(dot(input.normal_world,-lightDir));
	//_diffuse = diffuseFactor * material.diffuse * dirLight.diffuse;
	//// A = A
	//_ambient = material.ambient * dirLight.ambient;
	//
	//// I = A + D * N.L + (R.V)^n
	//float4 I = input.color * (_ambient + _diffuse) + _specular;
	//
	//return I;


	//tutorial
	float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 spec = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float3 toEye = normalize(eyePos - input.pos_W); //V

	// 光向量与照射方向相反
	float3 lightVec = -dirLight.direction;

	// 添加环境光
	ambient = material.ambient * dirLight.ambient;

	// 添加漫反射光和镜面光
	float diffuseFactor = dot(lightVec, input.normal_world);

	// 展开，避免动态分支
	
	float3 v = reflect(-lightVec, input.normal_world);
	float specFactor = pow(max(dot(v, toEye), 0.0f), material.specular.w);

	diffuse = diffuseFactor * material.diffuse * dirLight.diffuse;
	spec = specFactor * material.specular * dirLight.specular;

	float4 litColor = input.color * (ambient + diffuse) + spec;

	litColor.a = material.diffuse.a * input.color.a;

	return litColor;
}