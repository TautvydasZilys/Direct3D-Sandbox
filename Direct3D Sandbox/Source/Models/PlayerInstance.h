#pragma once

#include "IModelInstance.h"
#include "ZombieInstanceBase.h"
#include "Source\Audio\Sound.h"
#include "Source\CameraControllers\FPSController.h"
#include "Source\Games\ZombieSurvival\Highscore.h"

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
	FPSController m_CameraController;
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
	Font& m_SmallFont;

	Sound m_AmbientSound;
	Sound m_GameOverSound;

	Highscore m_Highscore;
	bool m_AchievedHighscore;
	
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

	void RenderHighscore(RenderParameters& renderParameters);

	void StartGame();
	void GameOver();

public:
	PlayerInstance(Camera& playerCamera);
	virtual ~PlayerInstance();
	
	virtual void Update(const RenderParameters& renderParameters);
	virtual void Render3D(RenderParameters& renderParameters) { }
	virtual void Render2D(RenderParameters& renderParameters);

	inline GameState GetGameState() const { return m_GameState; }
	inline const DirectX::XMFLOAT3& GetPosition() const { return m_CameraController.GetPosition(); }
	void TakeDamage(float damage);
};

