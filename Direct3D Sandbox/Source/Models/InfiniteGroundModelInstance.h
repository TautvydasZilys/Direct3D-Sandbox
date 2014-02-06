#pragma once

#include "ModelInstance.h"

class InfiniteGroundModelInstance :
	public ModelInstance
{
private:
	DirectX::XMFLOAT2 m_Scale;
	DirectX::XMFLOAT2 m_uvTiling;

public:
	InfiniteGroundModelInstance(const ModelParameters& modelParameters, const wstring& texturePath, DirectX::XMFLOAT2 uvTiling);
	virtual ~InfiniteGroundModelInstance();

	virtual void Render(RenderParameters& renderParameters);
};

