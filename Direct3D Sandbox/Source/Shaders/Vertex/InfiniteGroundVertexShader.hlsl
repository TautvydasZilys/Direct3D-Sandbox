cbuffer MatrixBuffer
{
	matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};

cbuffer GroundInfoBuffer
{
	float2 groundScale;
	float2 uvTiling;
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

	float2 groundPosition;

	output.position = mul(input.position, worldMatrix);
	groundPosition = output.position.xz - input.position.xz;

	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	output.tex = input.tex + groundPosition * uvTiling / groundScale;

	return output;
}