#include "PrecompiledHeader.h"
#include "InfiniteGroundModelInstance.h"
#include "Source\Graphics\IShader.h"

InfiniteGroundModelInstance::InfiniteGroundModelInstance(const ModelParameters& modelParameters, const wstring& texturePath, 
														 const wstring& normalMapPath, DirectX::XMFLOAT2 uvTiling) :
	CameraPositionLockedModelInstance(IShader::GetShader(ShaderType::INFINITE_GROUND_SHADER), L"Assets\\Models\\ground.model", modelParameters, 
		texturePath, normalMapPath, TypedDimensions<bool>(true, false, true)),
	m_uvTiling(uvTiling)
{
	m_Scale.x = modelParameters.scale.x;
	m_Scale.y = modelParameters.scale.z;
}

InfiniteGroundModelInstance::~InfiniteGroundModelInstance()
{
}

void InfiniteGroundModelInstance::Render3D(RenderParameters& renderParameters)
{
	renderParameters.groundScale = m_Scale;
	renderParameters.uvTiling = m_uvTiling;

	CameraPositionLockedModelInstance::Render3D(renderParameters);
}