#pragma once

#include "ModelInstance3D.h"

class ZombieInstanceBase;
class WeaponInstance :
	public ModelInstance3D
{
private:
	ModelInstance& m_Crosshair;
	float m_LastShot;

public:
	WeaponInstance();
	virtual ~WeaponInstance();

	int Fire(const vector<shared_ptr<ZombieInstanceBase>>& zombies, const DirectX::XMFLOAT3& playerPosition);

	static const DirectX::XMFLOAT3 kWeaponPositionOffset;
};

