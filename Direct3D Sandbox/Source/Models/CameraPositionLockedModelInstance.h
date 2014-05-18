#pragma once

#include "ModelInstance3D.h"

class CameraPositionLockedModelInstance :
	public ModelInstance3D
{
private:
	DirectX::XMFLOAT3 m_OriginalPosition;
	TypedDimensions<bool> m_LockedDimensions;

public:
	CameraPositionLockedModelInstance(IShader& shader, const wstring& modelPath, const ModelParameters& modelParameters, 
		TypedDimensions<bool> lockedDimentions);
	CameraPositionLockedModelInstance(IShader& shader, const wstring& modelPath, const ModelParameters& modelParameters, const wstring& texturePath,
		TypedDimensions<bool> lockedDimentions);
	CameraPositionLockedModelInstance(IShader& shader, const wstring& modelPath, const ModelParameters& modelParameters, const wstring& texturePath,
		const wstring& normalMapPath, TypedDimensions<bool> lockedDimentions);
	virtual ~CameraPositionLockedModelInstance();
	
	virtual void Update(const RenderParameters& renderParameters) { }
	virtual void Render3D(RenderParameters& renderParameters);
};