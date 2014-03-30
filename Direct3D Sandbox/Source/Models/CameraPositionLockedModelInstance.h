#pragma once

#include "ModelInstance.h"

template <typename T>
struct TypedDimensions;

class CameraPositionLockedModelInstance :
	public ModelInstance
{
private:
	DirectX::XMFLOAT3 m_OriginalPosition;
	TypedDimensions<bool> m_LockedDimensions;

public:
	CameraPositionLockedModelInstance(IShader& shader, const wstring& modelPath, const ModelParameters& modelParameters, TypedDimensions<bool> lockedDimentions);
	CameraPositionLockedModelInstance(IShader& shader, const wstring& modelPath, const ModelParameters& modelParameters, const wstring& texturePath, 
		TypedDimensions<bool> lockedDimentions);
	virtual ~CameraPositionLockedModelInstance();

	void SetPosition(const DirectX::XMFLOAT3& position);

	virtual void UpdateAndRender(RenderParameters& renderParameters);
};