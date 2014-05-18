#include "PrecompiledHeader.h"
#include "CameraPositionLockedModelInstance.h"


CameraPositionLockedModelInstance::CameraPositionLockedModelInstance(IShader& shader, const wstring& modelPath, const ModelParameters& modelParameters, 
																		TypedDimensions<bool> lockedDimentions) :
	ModelInstance3D(shader, modelPath, modelParameters),
	m_OriginalPosition(modelParameters.position),
	m_LockedDimensions(lockedDimentions)
{
}

CameraPositionLockedModelInstance::CameraPositionLockedModelInstance(IShader& shader, const wstring& modelPath, 
		const ModelParameters& modelParameters,	const wstring& texturePath, TypedDimensions<bool> lockedDimentions) :
	ModelInstance3D(shader, modelPath, modelParameters, texturePath),
	m_OriginalPosition(modelParameters.position),
	m_LockedDimensions(lockedDimentions)
{
}

CameraPositionLockedModelInstance::CameraPositionLockedModelInstance(IShader& shader, const wstring& modelPath, 
		const ModelParameters& modelParameters,	const wstring& texturePath, const wstring& normalMapPath, TypedDimensions<bool> lockedDimentions) :
	ModelInstance3D(shader, modelPath, modelParameters, texturePath, normalMapPath),
	m_OriginalPosition(modelParameters.position),
	m_LockedDimensions(lockedDimentions)
{
}

CameraPositionLockedModelInstance::~CameraPositionLockedModelInstance()
{
}

void CameraPositionLockedModelInstance::Render3D(RenderParameters& renderParameters)
{
	if (m_LockedDimensions.x)
	{
		m_Parameters.position.x = renderParameters.cameraPosition.x + m_OriginalPosition.x;
	}

	if (m_LockedDimensions.y)
	{
		m_Parameters.position.y = renderParameters.cameraPosition.y + m_OriginalPosition.y;
	}

	if (m_LockedDimensions.z)
	{
		m_Parameters.position.z = renderParameters.cameraPosition.z + m_OriginalPosition.z;
	}

	DirtyWorldMatrix();
	ModelInstance3D::Render3D(renderParameters);
}