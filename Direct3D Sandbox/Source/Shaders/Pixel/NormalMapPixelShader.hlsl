Texture2D Texture;
Texture2D NormalMap;
SamplerState WrapSampler;

cbuffer LightingBuffer
{
    float3 lightDirection;
    float3 lightColor;
	float3 ambientColor;
};

struct PixelInput
{
    float4 position : SV_POSITION;
    float2 tex : TEXTURECOORDINATES;
	float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 binormal : BINORMAL;
};

float4 main(PixelInput input) : SV_TARGET
{
    float lightIntensity;
    float4 color;
	float4 normalMap;

    color = Texture.Sample(WrapSampler, input.tex);

	normalMap = NormalMap.Sample(WrapSampler, input.tex) * 2.0f - 1.0f;
    input.normal = -normalMap.x * normalize(input.tangent) + -normalMap.y * normalize(input.binormal) + normalMap.z * normalize(input.normal);
	input.normal = normalize(input.normal);
    lightIntensity = saturate(dot(input.normal, lightDirection));
	color *= float4(saturate(lightColor * lightIntensity + ambientColor), 1.0f);

    return color;
}
