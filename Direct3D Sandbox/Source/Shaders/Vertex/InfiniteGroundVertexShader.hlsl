cbuffer MatrixBuffer
{
	matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
	matrix inversedTransposedWorldMatrix;
	float3 cameraPosition;
	float padding;
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
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);
	
	output.tex = uvTiling * (input.tex + cameraPosition.xz / groundScale);
    output.normal = -mul(input.normal, (float3x3)inversedTransposedWorldMatrix);

	return output;
}