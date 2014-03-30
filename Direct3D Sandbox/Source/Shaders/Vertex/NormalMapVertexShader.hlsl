cbuffer MatrixBuffer
{
	matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
	matrix inversedTransposedWorldMatrix;
};

struct VertexInput
{
    float4 position : POSITION;
	float2 tex : TEXTURECOORDINATES;
	float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 binormal : BINORMAL;
};

struct PixelInput
{
    float4 position : SV_POSITION;
    float2 tex : TEXTURECOORDINATES;
	float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 binormal : BINORMAL;
};

PixelInput main(VertexInput input)
{
    PixelInput output;

	output.position = mul(input.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

    output.tex = input.tex;
    output.normal = -mul(input.normal, (float3x3)inversedTransposedWorldMatrix);
	output.tangent = mul(input.tangent, (float3x3)inversedTransposedWorldMatrix);
	output.binormal = mul(input.binormal, (float3x3)inversedTransposedWorldMatrix);

    return output;
}