#include "Structures.hlsli"

//ps cbuffer slot 0
cbuffer PSConstantBuffer : register(b0) {
	DirectionalLight dirLight;
	Material material;
	float3 eyePos;
	float padding;
};

struct ColorPixelInput
{
	float4 pos_H : SV_POSITION;
	float3 pos_W : POSITION;
	float4 color : COLOR;
	float3 normal_world : NORMAL;
	float4 lightViewPosition : TEXCOORD1;
};


//t0 for normal texture
Texture2D ShadowTexture : register(t1);
//s0 for wrap sampler
SamplerState ClampLinearSampler : register(s1);

float4 main(ColorPixelInput input) : SV_TARGET{
	//Gouraud shading
	//return Input.color

	//Phong shading
	//init varibles
	float K_a = material.ambient; //0.7;
	float K_d = material.diffuse; //0.8;
	float K_s = material.specular; //0.3;
	float L_a = dirLight.ambient; //1.0
	float L_d = dirLight.diffuse; //1.0
	float L_s = dirLight.specular; //1.0


	float3 _lightDir = normalize(dirLight.direction); //L
	float3 _viewDir = normalize(eyePos - input.pos_W); //V

	//calculate directional light here
	// A = A
	float _ambient_factor = K_a * L_a;
	// D = (N.L)
	float _diffuse_factor = max(dot(input.normal_world, _lightDir), 0.0) * K_d * L_d;;

	// R = 2 * (N.L) * N - L
	float3 R = normalize(2 * max(dot(input.normal_world, _lightDir),0) * input.normal_world - _lightDir);
	
	// Spec = (R.V)^n
	float _specular_factor = pow(max(dot(R, _viewDir),0), 8.0) * K_s * L_s;
	
	// I = A + D * N.L + (R.V)^n
	float4 I = input.color * (_ambient_factor + _diffuse_factor + _specular_factor);
	I.a = 1.0;

	//draw shadow
	float bias = 0.00002f;
	float depthValue;
	float lightDepthValue;
	float2 projectTexCoord;
	// Calculate the projected texture coordinates.
	projectTexCoord.x = input.lightViewPosition.x / input.lightViewPosition.w * 0.5f + 0.5f;
	projectTexCoord.y = -input.lightViewPosition.y / input.lightViewPosition.w * 0.5f + 0.5f;
	// Determine if the projected coordinates are in the 0 to 1 range.  If so then this pixel is in the view of the light.
	if ((saturate(projectTexCoord.x) == projectTexCoord.x) && (saturate(projectTexCoord.y) == projectTexCoord.y))
	{
		// Sample the shadow map depth value from the depth texture using the sampler at the projected texture coordinate location.
		depthValue = ShadowTexture.Sample(ClampLinearSampler, projectTexCoord).r;

		// Calculate the depth of the light.
		lightDepthValue = input.lightViewPosition.z / input.lightViewPosition.w;

		// Subtract the bias from the lightDepthValue.
		lightDepthValue = lightDepthValue - bias;

		// Compare the depth of the shadow map value and the depth of the light to determine whether to shadow or to light this pixel.
		// If the light is in front of the object then light the pixel, if not then shadow this pixel since an object (occluder) is casting a shadow on it.
		
		//only ambient light for shadow
		if (lightDepthValue >= depthValue)
		{
			I = input.color * _ambient_factor;
		}
	}

	return I;
}