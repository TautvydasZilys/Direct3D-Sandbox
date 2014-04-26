cbuffer PerModelInstanceBuffer
{
	matrix worldMatrix;
	matrix inversedTransposedWorldMatrix;
};

cbuffer PerFrameBuffer
{
	float time;
	float3 padding;
    matrix viewProjectionMatrix;
};

struct VertexInput
{
    float4 position : POSITION;
	float2 tex : TEXTURECOORDINATES;
	float3 normal : NORMAL;
};

struct PixelInput
{
    float4 position : SV_POSITION;
    float2 tex : TEXTURECOORDINATES;
	float3 normal : NORMAL;
};

PixelInput main(VertexInput input)
{
	PixelInput output;

	output.position = mul(input.position, worldMatrix);

	output.position.w *= 2.0f + sin(time);

	output.position = mul(output.position, viewProjectionMatrix);

	output.tex = 4.0f * input.tex;
	output.tex.x += time;

    output.normal = -mul(input.normal, (float3x3)inversedTransposedWorldMatrix);

	return output;
}