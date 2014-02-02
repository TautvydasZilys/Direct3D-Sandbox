#include "PrecompiledHeader.h"
#include "DirectionalLight.h"
#include "Parameters.h"

DirectionalLight::DirectionalLight(DirectX::XMFLOAT3 direction, DirectX::XMFLOAT3 color, DirectX::XMFLOAT3 ambientColor, float specularIntensity) :
	m_Color(color),
	m_AmbientColor(ambientColor),
	m_SpecularIntensity(specularIntensity)
{	
	float length = sqrt(direction.x * direction.x + direction.y * direction.y + direction.z * direction.z);
	m_Direction.x = direction.x / length;
	m_Direction.y = direction.y / length;
	m_Direction.z = direction.z / length;
}

DirectionalLight::~DirectionalLight(void)
{
}

void DirectionalLight::SetRenderParameters(RenderParameters& renderParameters)
{
	renderParameters.lightDirection = m_Direction;
	renderParameters.lightColor = m_Color;
	renderParameters.ambientColor = m_AmbientColor;
	renderParameters.specularIntensity = m_SpecularIntensity;
}