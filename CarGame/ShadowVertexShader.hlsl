#include "Structures.hlsli"

struct ShadowVertexInput {
	float3 pos : POSITION;
	float4 color : COLOR;
	float3 normal_local: NORMAL;
};

struct ShadowVertexOutput {
	float3 pos_W;
	float4 pos_H;
};


ShadowVertexOutput main(ShadowVertexInput input){
	ShadowVertexOutput output;
	float4 pos_H = float4(input.pos, 1.0f);

	pos_H = mul(pos_H, world);
	pos_H = mul(pos_H, view);
	pos_H = mul(pos_H, orthoProjection);

	output.pos_H = pos_H; //transformed homogenuous position
	output.pos_W = pos_H.xyz; //transoformed position
	return output;
}