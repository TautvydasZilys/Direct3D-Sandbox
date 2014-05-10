#pragma once

#include "CameraPositionLockedModelInstance.h"

class InfiniteGroundModelInstance :
	public CameraPositionLockedModelInstance
{
private:
	DirectX::XMFLOAT2 m_Scale;
	DirectX::XMFLOAT2 m_uvTiling;

public:
	InfiniteGroundModelInstance(const ModelParameters& modelParameters, const wstring& texturePath, DirectX::XMFLOAT2 uvTiling);
	virtual ~InfiniteGroundModelInstance();

	virtual void UpdateAndRender3D(RenderParameters& renderParameters);
};

