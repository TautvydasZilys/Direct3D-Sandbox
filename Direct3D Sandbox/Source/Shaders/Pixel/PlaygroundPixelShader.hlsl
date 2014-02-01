Texture2D Texture;
SamplerState WrapSampler;

struct PixelInput
{
    float4 position : SV_POSITION;
    float2 tex : TEXTURECOORDINATES;
};

float4 main(PixelInput input) : SV_TARGET
{
	return Texture.Sample(WrapSampler, input.tex);
}