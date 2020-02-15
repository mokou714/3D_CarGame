#include "Structures.hlsli"

struct TexPixelInput
{
	float4 pos_H : SV_POSITION;
	float3 pos_W : POSITION;
	float3 normal_world : NORMAL;
	float2 tex : TEXCOORD;
};

Texture2D Texture : register(t0);
SamplerState SamLinear : register(s0);

float4 main(TexPixelInput input) : SV_TARGET
{
	float4 texColor = Texture.Sample(SamLinear, input.tex);

	return texColor;
}