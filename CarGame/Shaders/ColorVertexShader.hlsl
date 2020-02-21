#include "Structures.hlsli"

//vs cbuffer slot 0
cbuffer VSConstantBuffer : register(b0) {
	matrix world;
	matrix view;
	matrix projection;
	matrix inv_world_view; //for normal calculation
};


//vs cbuffer slot1
cbuffer OrthoConstantBuffer : register(b1) {
	matrix orthoWorld;
	matrix orthoView;
	matrix orthoProjection;
};


ColorVertexOutput main(ColorVertexInput vertexInput) {
	ColorVertexOutput output;
	float4 pos_H = float4(vertexInput.pos, 1.0f);

	pos_H = mul(pos_H, world);
	pos_H = mul(pos_H, view);
	pos_H = mul(pos_H, projection);

	//camera view
	output.pos_H = pos_H; //transformed homogenuous position
	output.pos_W = pos_H.xyz; //transoformed position

	//light view
	float4 light_pos_H = float4(vertexInput.pos, 1.0f);
	light_pos_H = mul(light_pos_H, orthoWorld);
	light_pos_H = mul(light_pos_H, orthoView);
	light_pos_H = mul(light_pos_H, orthoProjection);
	output.lightViewPosition = light_pos_H;

	//pass the same color
	output.color = vertexInput.color;
	
	//transform normal
	output.normal_world = normalize(mul(vertexInput.normal_local, inv_world_view));

	return output;
}