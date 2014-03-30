#pragma once

#include "IModelInstance.h"
#include "ZombieInstance.h"
#include "Source\Cameras\Camera.h"

class PlayerInstance :
	public IModelInstance
{
private:
	const Camera& m_Camera;
	vector<weak_ptr<ZombieInstance>> m_Zombies;
	float m_LastSpawnTime;

public:
	PlayerInstance(const Camera& playerCamera);
	virtual ~PlayerInstance();

	virtual void UpdateAndRender(RenderParameters& renderParameters);

	inline DirectX::XMFLOAT3 GetPosition() const { return m_Camera.GetPosition(); }
};

