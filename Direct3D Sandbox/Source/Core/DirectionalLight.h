#pragma once

struct RenderParameters;

class DirectionalLight
{
private:
	DirectX::XMFLOAT3 m_Direction;
	DirectX::XMFLOAT3 m_Color;
	DirectX::XMFLOAT3 m_AmbientColor;
	float m_SpecularIntensity;

public:
	DirectionalLight(DirectX::XMFLOAT3 direction, DirectX::XMFLOAT3 color, DirectX::XMFLOAT3 ambientColor, float specularIntensity);
	~DirectionalLight();

	void SetRenderParameters(RenderParameters& renderParameters);
};

