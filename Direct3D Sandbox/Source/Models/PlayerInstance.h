#pragma once

#include "IModelInstance.h"
#include "ZombieInstanceBase.h"
#include "Source\Cameras\Camera.h"

class PlayerInstance :
	public IModelInstance
{
private:
	const Camera& m_Camera;
	vector<weak_ptr<ZombieInstanceBase>> m_Zombies;
	float m_StartTime;
	float m_LastSpawnTime;

	void SpawnZombie();

public:
	PlayerInstance(const Camera& playerCamera);
	virtual ~PlayerInstance();
	
	virtual void UpdateAndRender(RenderParameters& renderParameters);
	virtual void UpdateAndRender2D(RenderParameters& renderParameters);

	inline DirectX::XMFLOAT3 GetPosition() const { return m_Camera.GetPosition(); }
};

