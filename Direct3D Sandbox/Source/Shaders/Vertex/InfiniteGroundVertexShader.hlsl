cbuffer MatrixBuffer
{
	matrix worldViewProjectionMatrix;
	matrix inversedTransposedWorldMatrix;
	float3 cameraPosition;
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
	
	output.position = mul(input.position, worldViewProjectionMatrix);
	
	output.tex = uvTiling * (input.tex + cameraPosition.xz / groundScale);
    output.normal = -mul(input.normal, (float3x3)inversedTransposedWorldMatrix);
    output.tangent = -mul(input.tangent, (float3x3)inversedTransposedWorldMatrix);
    output.binormal = -mul(input.binormal, (float3x3)inversedTransposedWorldMatrix);

	return output;
}