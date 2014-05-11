Texture2D Texture;
SamplerState WrapSampler;

cbuffer ColorBuffer
{
	float4 color;
}

struct PixelInput
{
    float4 position : SV_POSITION;
    float2 tex : TEXTURECOORDINATES;
};

float4 main(PixelInput input) : SV_TARGET
{
	return color * Texture.Sample(WrapSampler, input.tex);
}