cbuffer OrthoConstantBuffer : register(b2) {
	matrix orthoWorld;
	matrix orthoView;
	matrix orthoProjection;
};

struct ShadowVertexInput {
	float3 pos : POSITION;
};

struct ShadowVertexOutput {
	float4 pos_H : SV_POSITION;
};


ShadowVertexOutput main(ShadowVertexInput input){
	ShadowVertexOutput output;
	float4 pos_H = float4(input.pos, 1.0f);

	pos_H = mul(pos_H, orthoWorld);
	pos_H = mul(pos_H, orthoView);
	pos_H = mul(pos_H, orthoProjection);

	output.pos_H = pos_H; //transformed homogenuous position

	return output;
}