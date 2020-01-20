cbuffer VSConstantBuffer : register(b0) {
	matrix world;
	matrix view;
	matrix projection;
};

cbuffer PSConstantBuffer : register(b1) {
	//matrix world;
	//matrix view;
	//matrix projection;
};

struct VertexShaderInput {
	float3 pos : POSITION;
	float4 color : COLOR;
	float3 normal: NORMAL;
};

struct VertexShaderOutput {
	float4 pos : SV_POSITION;
	float4 color : COLOR;
	float3 normal : NORMAL;
};

VertexShaderOutput main(VertexShaderInput vertexInput) {
	VertexShaderOutput output;
	float4 pos = float4(vertexInput.pos, 1.0f);

	//Transform the vertex position into projected space.
	pos = mul(pos, world);
	pos = mul(pos, view);
	pos = mul(pos, projection);

	output.pos = pos;

	//Pass the color through without modification.
	output.color = vertexInput.color;
	
	//Pass normal as it is, for now
	output.normal = vertexInput.normal;
	

	return output;
}