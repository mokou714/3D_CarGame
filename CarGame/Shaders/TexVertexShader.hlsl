#include "Structures.hlsli"

TexVertexOutput main(TexVertexInput vertexInput) {
	TexVertexOutput output;
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

	//transform normal
	output.normal_world = normalize(mul(vertexInput.normal_local, inv_world_view));

	//pass texcoord
	output.tex = vertexInput.tex;

	return output;
}