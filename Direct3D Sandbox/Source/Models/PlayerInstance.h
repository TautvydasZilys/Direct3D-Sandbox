#pragma once

#include "IModelInstance.h"
#include "ZombieInstanceBase.h"
#include "Source\Cameras\Camera.h"

class WeaponInstance;

enum GameState
{
	NotStarted,
	Playing,
	GameOver
};

class Font;
class PlayerInstance :
	public IModelInstance
{
private:
	Camera& m_Camera;
	shared_ptr<WeaponInstance> m_Weapon;
	vector<shared_ptr<ZombieInstanceBase>> m_Zombies;
	float m_StartTime;
	float m_DeathTime;
	float m_LastSpawnTime;

	float m_SpawnInterval;
	float m_SpawnCount;

	float m_Health;
	int m_ZombiesKilled;
	
	GameState m_GameState;
	Font& m_BoldFont;

	void UpdateInput(float frameTime);
	void UpdateWeapon();

	void SpawnRandomZombie();
	void SpawnZombie();
	void SpawnSuperZombie();
	
	void UpdateStateNotStarted(const RenderParameters& renderParameters);
	void UpdateStatePlaying(const RenderParameters& renderParameters);
	void UpdateStateGameOver(const RenderParameters& renderParameters);

	void RenderStateNotStarted2D(RenderParameters& renderParameters);
	void RenderStatePlaying2D(RenderParameters& renderParameters);
	void RenderStateGameOver2D(RenderParameters& renderParameters);

	void StartGame();
	void GameOver();

public:
	PlayerInstance(Camera& playerCamera);
	virtual ~PlayerInstance();
	
	virtual void Update(const RenderParameters& renderParameters);
	virtual void Render3D(RenderParameters& renderParameters) { }
	virtual void Render2D(RenderParameters& renderParameters);

	inline GameState GetGameState() const { return m_GameState; }
	inline DirectX::XMFLOAT3 GetPosition() const { return m_Camera.GetPosition(); }
	void TakeDamage(float damage);
};

