Texture2D Texture;
SamplerState MirrorSampler;

cbuffer LightingBuffer
{
    float3 lightDirection;
    float3 lightColor;
	float3 ambientColor;
};

struct PixelInput
{
    float4 position : SV_POSITION;
    float2 tex : TEXTURECOORDINATES;
	float3 normal : NORMAL;
};

float4 main(PixelInput input) : SV_TARGET
{
	float3 lightIntensity;
	float4 color;
	
	color = Texture.Sample(MirrorSampler, input.tex);

	input.normal = normalize(input.normal);
    lightIntensity = saturate(dot(input.normal, lightDirection));
	color *= float4(saturate(lightColor * lightIntensity + ambientColor), 1.0f);
	
	return color;
}