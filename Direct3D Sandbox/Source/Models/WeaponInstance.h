#pragma once

#include "ModelInstance3D.h"

class ZombieInstanceBase;
class WeaponInstance :
	public ModelInstance3D
{
private:
	ModelInstance& m_Crosshair;

public:
	WeaponInstance();
	virtual ~WeaponInstance();

	void Fire(const vector<weak_ptr<ZombieInstanceBase>>& zombies, const DirectX::XMFLOAT3& playerPosition);

	static const DirectX::XMFLOAT3 kWeaponPositionOffset;
};

