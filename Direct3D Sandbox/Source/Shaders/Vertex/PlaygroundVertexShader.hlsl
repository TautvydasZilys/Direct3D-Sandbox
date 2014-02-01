cbuffer MatrixBuffer
{
	matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};

cbuffer TimeBuffer
{
	float time;
	float3 padding;
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

	output.position = mul(input.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	output.tex = input.tex;
	output.tex.x += time * 0.25f;

	return output;
}