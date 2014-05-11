#pragma once

#include "ModelInstance3D.h"

class LaserProjectileInstance :
	public ModelInstance3D
{
private:
	float m_SpawnedAt;
	float m_TransitionProgress;
	DirectX::XMFLOAT3 m_RayDirectionNormalized;

	LaserProjectileInstance(const ModelParameters& modelParameters, const DirectX::XMVECTOR& rayDirection);

public:
	virtual ~LaserProjectileInstance();

	virtual void Update(const RenderParameters& renderParameters);
	virtual void Render3D(RenderParameters& renderParameters);
	static void Spawn(const DirectX::XMVECTOR& source, const DirectX::XMVECTOR& target);
};

