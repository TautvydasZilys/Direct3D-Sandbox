#pragma once

#include "ModelInstance3D.h"

class WeaponInstance :
	public ModelInstance3D
{
private:
	ModelInstance& m_Crosshair;

public:
	WeaponInstance();
	virtual ~WeaponInstance();

	void Fire();

	static const DirectX::XMFLOAT3 kWeaponPositionOffset;
};

