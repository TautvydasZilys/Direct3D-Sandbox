#pragma once

#include "ModelInstance3D.h"
#include "Source\Audio\AudioEmitter.h"
#include "Source\Audio\Sound.h"

class ZombieInstanceBase;
class WeaponInstance :
	public ModelInstance3D
{
private:
	ModelInstance& m_Crosshair;
	float m_LastShot;

	Sound m_weaponTriggerSound;
	AudioEmitter m_AudioEmitter;

public:
	WeaponInstance();
	virtual ~WeaponInstance();

	int Fire(const vector<shared_ptr<ZombieInstanceBase>>& zombies, const DirectX::XMFLOAT3& playerPosition);

	static const DirectX::XMFLOAT3 kWeaponPositionOffset;
};

