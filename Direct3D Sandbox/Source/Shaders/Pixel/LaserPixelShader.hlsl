cbuffer LaserBuffer
{
	float4 color;
	float3 rayViewDirection;
}

struct PixelInput
{
    float4 position : SV_POSITION;
	float3 normal : NORMAL;
};

float4 main(PixelInput input) : SV_TARGET
{
	float4 finalColor;
	float rayBrightness;
	
	rayBrightness = 1.0f - abs(dot(normalize(input.normal), rayViewDirection));

	finalColor = color * pow(rayBrightness, 0.6f);
	finalColor.a = 1.0f;
	
	return finalColor;
}