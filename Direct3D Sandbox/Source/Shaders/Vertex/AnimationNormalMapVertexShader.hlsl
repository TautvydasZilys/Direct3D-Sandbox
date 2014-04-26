cbuffer MatrixBuffer
{
	matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
	matrix inversedTransposedWorldMatrix;
	float currentFrameProgress;
	float3 padding;
};

struct VertexInput
{
    float4 position : POSITION;
	float2 tex : TEXTURECOORDINATES;
	float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 binormal : BINORMAL;
	
	float4 position2 : POSITIONSECONDARY;
	float3 normal2 : NORMALSECONDARY;
	float3 tangent2 : TANGENTSECONDARY;
	float3 binormal2 : BINORMALSECONDARY;
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

	output.position = mul(lerp(input.position, input.position2, currentFrameProgress), worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

    output.tex = input.tex;
    output.normal = -mul(lerp(input.normal, input.normal2, currentFrameProgress), (float3x3)inversedTransposedWorldMatrix);
	output.tangent = mul(lerp(input.tangent, input.tangent2, currentFrameProgress), (float3x3)inversedTransposedWorldMatrix);
	output.binormal = mul(lerp(input.binormal, input.binormal2, currentFrameProgress), (float3x3)inversedTransposedWorldMatrix);

    return output;
}