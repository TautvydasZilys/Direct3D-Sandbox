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

	output.position.w *= 2.0f + sin(time);

	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	output.tex = 4.0f * input.tex;
	output.tex.x += time;

	return output;
}