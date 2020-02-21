struct DirectionalLight
{
	float ambient;
	float diffuse;
	float specular;
	float padding1;
	float3 direction;
	float padding2;
};

struct Material
{
	float ambient;
	float diffuse;
	float specular;
	float padding;
};

struct ColorVertexInput {
	float3 pos : POSITION;
	float4 color : COLOR;
	float3 normal_local: NORMAL;
};

struct ColorVertexOutput {
	float4 pos_H : SV_POSITION;
	float3 pos_W : POSITION;
	float4 color : COLOR;
	float3 normal_world : NORMAL;
	float4 lightViewPosition : TEXCOORD1;
};

struct TexVertexInput {
	float3 pos : POSITION;
	float3 normal_local: NORMAL;
	float2 tex : TEXCOORD;
};

struct TexVertexOutput {
	float4 pos_H : SV_POSITION;
	float3 pos_W : POSITION;
	float3 normal_world : NORMAL;
	float2 tex : TEXCOORD;
	float4 lightViewPosition : TEXCOORD1;
};