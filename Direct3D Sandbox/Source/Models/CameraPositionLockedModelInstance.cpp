#include "PrecompiledHeader.h"
#include "CameraPositionLockedModelInstance.h"


CameraPositionLockedModelInstance::CameraPositionLockedModelInstance(IShader& shader, const wstring& modelPath, const ModelParameters& modelParameters, 
																		TypedDimensions<bool> lockedDimentions) :
	ModelInstance(shader, modelPath, modelParameters),
	m_LockedDimensions(lockedDimentions)
{
}

CameraPositionLockedModelInstance::CameraPositionLockedModelInstance(IShader& shader, const wstring& modelPath, const ModelParameters& modelParameters, 
																		const wstring& texturePath, TypedDimensions<bool> lockedDimentions) :
	ModelInstance(shader, modelPath, modelParameters, texturePath),
	m_LockedDimensions(lockedDimentions)
{
}

CameraPositionLockedModelInstance::~CameraPositionLockedModelInstance()
{
}

void CameraPositionLockedModelInstance::Render(RenderParameters& renderParameters)
{
	if (m_LockedDimensions.x)
	{
		m_Parameters.position.x = renderParameters.cameraPosition.x;
	}

	if (m_LockedDimensions.y)
	{
		m_Parameters.position.y = renderParameters.cameraPosition.y;
	}

	if (m_LockedDimensions.z)
	{
		m_Parameters.position.z = renderParameters.cameraPosition.z;
	}
	
	Initialize();

	ModelInstance::Render(renderParameters);
}