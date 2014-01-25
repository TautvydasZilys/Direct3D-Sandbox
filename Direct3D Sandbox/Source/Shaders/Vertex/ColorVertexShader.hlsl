cbuffer MatrixBuffer
{
	matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
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

	output.position = mul(input.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	output.color = color;

	return output;
}