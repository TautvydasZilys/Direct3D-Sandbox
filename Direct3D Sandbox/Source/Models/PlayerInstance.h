#pragma once

#include "IModelInstance.h"
#include "ZombieInstanceBase.h"
#include "Source\Cameras\Camera.h"

class WeaponInstance;

class PlayerInstance :
	public IModelInstance
{
private:
	const Camera& m_Camera;
	WeaponInstance& m_Weapon;
	vector<weak_ptr<ZombieInstanceBase>> m_Zombies;
	float m_StartTime;
	float m_LastSpawnTime;
	
	void UpdateWeapon();

	void SpawnRandomZombie();
	void SpawnZombie();
	void SpawnSuperZombie();

public:
	PlayerInstance(const Camera& playerCamera);
	virtual ~PlayerInstance();
	
	virtual void UpdateAndRender3D(RenderParameters& renderParameters);
	virtual void UpdateAndRender2D(RenderParameters& renderParameters);

	inline DirectX::XMFLOAT3 GetPosition() const { return m_Camera.GetPosition(); }
};

