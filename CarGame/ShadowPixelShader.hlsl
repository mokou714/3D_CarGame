
struct ShadowPixelInput {
	float4 pos_H : SV_POSITION;
	float3 pos_W : POSITION;
};

float4 main(ShadowPixelInput input) :SV_TARGET{
	return float4(1.0,1.0,1.0,1.0);
}