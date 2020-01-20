struct DirectionalLight
{
	float4 ambient;
	float4 diffuse;
	float4 specular;
	float3 direction;
};

struct Material
{
	float4 ambient;
	float4 diffuse;
	float4 specular;
	float4 reflect;
};

cbuffer VSConstantBuffer : register(b0) {
	matrix world;
	matrix view;
	matrix projection;
	matrix inv_world_view; //for normal calculation
};

cbuffer PSConstantBuffer : register(b1) {
	DirectionalLight dirLight;
	Material material;
	float3 eyePos;
};

struct VertexShaderInput {
	float3 pos : POSITION;
	float4 color : COLOR;
	float3 normal_local: NORMAL;
};

struct VertexShaderOutput {
	float4 pos_H : SV_POSITION;
	float3 pos_W : POSITION;
	float4 color : COLOR;
	float3 normal_world : NORMAL;
};