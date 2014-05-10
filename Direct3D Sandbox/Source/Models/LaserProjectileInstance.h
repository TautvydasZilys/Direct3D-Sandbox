#pragma once

#include "ModelInstance3D.h"

class LaserProjectileInstance :
	public ModelInstance3D
{
private:
	float m_SpawnedAt;

	LaserProjectileInstance(const ModelParameters& modelParameters);

public:
	virtual ~LaserProjectileInstance();

	virtual void UpdateAndRender3D(RenderParameters& renderParameters);
	static void Spawn(const DirectX::XMVECTOR& source, const DirectX::XMVECTOR& target);
};

