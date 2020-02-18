#include "Structures.hlsli"

struct ShadowVertexInput {
	float3 pos : POSITION;
	float3 normal_local: NORMAL;
	float4 color : COLOR;
};

struct ShadowVertexOutput {
	float4 pos_H : SV_POSITION;
	float3 pos_W : POSITION;
};


ShadowVertexOutput main(ShadowVertexInput input){
	ShadowVertexOutput output;
	float4 pos_H = float4(input.pos, 1.0f);

	pos_H = mul(pos_H, orthoWorld);
	pos_H = mul(pos_H, orthoView);
	pos_H = mul(pos_H, orthoProjection);

	output.pos_H = pos_H; //transformed homogenuous position
	output.pos_W = pos_H.xyz; //transoformed position
	return output;
}