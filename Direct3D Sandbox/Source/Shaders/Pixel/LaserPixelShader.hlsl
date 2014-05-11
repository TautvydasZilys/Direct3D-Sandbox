cbuffer LaserBuffer
{
	float4 color;
	float3 rayViewDirection;
	float transitionProgress;
}

struct PixelInput
{
    float4 position : SV_POSITION;
	float3 normal : NORMAL;
};

float4 main(PixelInput input) : SV_TARGET
{
	float4 finalColor;
	float4 startColor, endColor;
	float rayBrightness;

	rayBrightness = pow(1.0f - abs(dot(normalize(input.normal), rayViewDirection)), 0.6f);
	startColor = color + (float4(1.0f, 1.0f, 1.0f, 1.0f) - color) * rayBrightness;
	endColor = color * rayBrightness;

	finalColor = lerp(startColor, endColor, transitionProgress);
	finalColor.a = 1.0f;
	
	return finalColor;
}