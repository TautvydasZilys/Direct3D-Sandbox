#pragma once

#include "ModelInstance3D.h"

class PlayerInstance;
class ZombieInstanceBase :
	public ModelInstance3D
{	
protected:
	const PlayerInstance& m_TargetPlayer;
	const float m_Speed;

	bool m_IsDead;
	float m_Health;
	float m_SpawnTime;
	float m_DeathTime;

	ZombieInstanceBase(IShader& shader, const wstring& modelPath, const wstring& texturePath, const wstring& normalMapPath,
		const ModelParameters& modelParameters, const PlayerInstance& targetPlayer, float speed);

	static ModelParameters GetRandomZombieParameters(const PlayerInstance& targetPlayer);

private:
	ZombieInstanceBase(const ModelInstance& other);					// Not implemented (no copying allowed)
	ZombieInstanceBase& operator=(const ModelInstance& other);		// Not implemented (no copying allowed)

public:
	virtual ~ZombieInstanceBase();
	virtual void UpdateAndRender3D(RenderParameters& renderParameters);

	bool IsDead() const { return m_IsDead; }
	bool TakeDamage(float damage);
};

