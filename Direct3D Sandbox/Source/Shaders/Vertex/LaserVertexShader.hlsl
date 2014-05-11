cbuffer MatrixBuffer
{
	matrix worldViewProjectionMatrix;
	matrix inversedTransposedWorldMatrix;
};

struct VertexInput
{
    float4 position : POSITION;
	float3 normal : NORMAL;
};

struct PixelInput
{
    float4 position : SV_POSITION;
	float3 normal : NORMAL;
};

PixelInput main(VertexInput input)
{
	PixelInput output;
	
	output.position = mul(input.position, worldViewProjectionMatrix);
    output.normal = -mul(input.normal, (float3x3)inversedTransposedWorldMatrix);

	return output;
}