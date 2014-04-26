cbuffer MatrixBuffer
{
	matrix worldViewProjectionMatrix;
	float4 color;
};

struct VertexInput
{
    float4 position : POSITION;
};

struct PixelInput
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

PixelInput main(VertexInput input)
{
	PixelInput output;

	output.position = mul(input.position, worldViewProjectionMatrix);
	output.color = color;

	return output;
}