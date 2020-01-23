#include "Structures.hlsli"

TexVertexOutput main(TexVertexInput vertexInput) {
	TexVertexOutput output;
	float4 pos_H = float4(vertexInput.pos, 1.0f);

	pos_H = mul(pos_H, world);
	pos_H = mul(pos_H, view);
	pos_H = mul(pos_H, projection);

	output.pos_H = pos_H; //transformed homogenuous position
	output.pos_W = pos_H.xyz; //transoformed position

	//transform normal
	output.normal_world = normalize(mul(vertexInput.normal_local, inv_world_view));

	//pass texcoord
	output.tex = vertexInput.tex;

	return output;
}