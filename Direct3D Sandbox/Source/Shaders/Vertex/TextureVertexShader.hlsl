cbuffer MatrixBuffer
{
	matrix worldViewProjectionMatrix;
};

struct VertexInput
{
    float4 position : POSITION;
	float2 tex : TEXTURECOORDINATES;
};

struct PixelInput
{
    float4 position : SV_POSITION;
    float2 tex : TEXTURECOORDINATES;
};

PixelInput main(VertexInput input)
{
	PixelInput output;

	output.position = mul(input.position, worldViewProjectionMatrix);
	output.tex = input.tex;

	return output;
}