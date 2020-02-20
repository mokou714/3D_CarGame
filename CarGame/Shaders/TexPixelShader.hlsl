#include "Structures.hlsli"

struct TexPixelInput
{
	float4 pos_H : SV_POSITION;
	float3 pos_W : POSITION;
	float3 normal_world : NORMAL;
	float2 tex : TEXCOORD;
	float4 lightViewPosition : TEXCOORD1;
};

Texture2D Texture : register(t0);
Texture2D ShadowTexture : register(t1);
SamplerState WrapLinearSampler : register(s0);
SamplerState ClampLinearSampler : register(s1);

float4 main(TexPixelInput input) : SV_TARGET
{
	//init varibles
	float4 _ambient = {0.0f, 0.0f, 0.0f, 0.0f};
	float4 _diffuse = { 0.0f, 0.0f, 0.0f, 0.0f};
	float4 _specular = { 0.0f, 0.0f, 0.0f, 0.0f};
	float3 _lightDir = normalize(dirLight.direction); //L
	float3 _viewDir = normalize(eyePos - input.pos_W); //V

	//calculate directional light here
	// A = A
	_ambient = material.ambient * dirLight.ambient;
	// D = (N.L)
	float diffuseFactor = dot(input.normal_world, _lightDir);

	if (diffuseFactor > 0.0f) {
		_diffuse = diffuseFactor * material.diffuse * dirLight.diffuse;
		// R = 2 * (N.L) * N - L
		float3 R = normalize(2 * dot(input.normal_world, _lightDir) * input.normal_world - _lightDir);
		// Spec = (R.V)^n
		float specularFactor = pow(dot(R, _viewDir), material.specular.w);
		_specular = specularFactor * material.specular * dirLight.specular;
	}

	float4 texColor = Texture.Sample(WrapLinearSampler, input.tex);
	// I = A + D * N.L + (R.V)^n
	float4 I = texColor * (_ambient + _diffuse + _specular);
	I.a = material.diffuse.a * texColor.a;


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
			I = texColor * _ambient;
		}
	}

	return I;
}