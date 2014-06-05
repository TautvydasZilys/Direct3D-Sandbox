#include "PrecompiledHeader.h"
#include "PlayerInstance.h"

#include "Constants.h"
#include "Source\Graphics\Font.h"
#include "Source\Graphics\IShader.h"
#include "System.h"

#include "SuperZombieInstance.h"
#include "WeaponInstance.h"
#include "ZombieInstance.h"

static const float kSmallestSpawnInterval = 0.5f;
static const float kHealthRegenerationRate = 0.02f;

PlayerInstance::PlayerInstance(Camera& playerCamera) :
	m_CameraController(playerCamera),
	m_GameState(GameState::NotStarted),
	m_BoldFont(Font::Get(L"Assets\\Fonts\\Segoe UI.font")),
	m_SmallFont(Font::Get(L"Assets\\Fonts\\Calibri.font")),
	m_AmbientSound(L"Assets\\Sounds\\Ambient.wav", true, false),
	m_GameOverSound(L"Assets\\Sounds\\GameOver.wav", false, false),
	m_Highscore(Highscore::Load()),
	m_AchievedHighscore(false)
{
	m_AmbientSound.Play();
}

PlayerInstance::~PlayerInstance()
{
	for (auto& zombie : m_Zombies)
	{
		System::GetInstance().RemoveModel(zombie.get());
	}

	if (m_GameState == GameState::Playing)
	{
		System::GetInstance().RemoveModel(m_Weapon.get());
	}
}

void PlayerInstance::Update(const RenderParameters& renderParameters)
{
	switch (m_GameState)
	{
	case GameState::NotStarted:
		UpdateStateNotStarted(renderParameters);
		break;

	case GameState::Playing:
		UpdateStatePlaying(renderParameters);
		break;

	case GameState::GameOver:
		UpdateStateGameOver(renderParameters);
		break;
	}
}

void PlayerInstance::Render2D(RenderParameters& renderParameters)
{
	switch (m_GameState)
	{
	case GameState::NotStarted:
		RenderStateNotStarted2D(renderParameters);
		break;

	case GameState::Playing:
		RenderStatePlaying2D(renderParameters);
		break;

	case GameState::GameOver:
		RenderStateGameOver2D(renderParameters);
		break;
	}
}

void PlayerInstance::UpdateStateNotStarted(const RenderParameters& renderParameters)
{
	auto& input = Input::GetInstance();

#if WINDOWS_PHONE	
	auto pinch = input.HandlePinchDisplacement();
#endif

#if !WINDOWS_PHONE
	if (input.IsKeyDown(VK_RETURN))
#else
	if (pinch > 0)
#endif
	{
		StartGame();
	}
}

void PlayerInstance::RenderStateNotStarted2D(RenderParameters& renderParameters)
{
	renderParameters.color = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	m_BoldFont.DrawText("  Welcome to\nZombie Siege!", renderParameters.screenWidth / 2 - 385, 
		renderParameters.screenHeight / 2 - 200, renderParameters, true);
	
#if !WINDOWS_PHONE
	auto text = "Press enter to begin";
#else
	auto text = "Pinch in to begin";
#endif
	Font::GetDefault().DrawText(text, renderParameters.screenWidth / 2 - 200, renderParameters.screenHeight / 2 + 300, renderParameters, true);

	RenderHighscore(renderParameters);
}

void PlayerInstance::UpdateStatePlaying(const RenderParameters& renderParameters)
{
	// Remove destroyed/dead zombies
	for (auto i = 0u; i < m_Zombies.size(); i++)
	{
		if (m_Zombies[i]->IsDead())
		{
			m_Zombies[i] = m_Zombies[m_Zombies.size() - 1];
			m_Zombies.pop_back();
			i--;
		}
	}

	if (renderParameters.time - m_LastSpawnTime >= m_SpawnInterval && static_cast<int>(m_Zombies.size()) < Constants::MaxZombies)
	{
		for (int i = 0; i < m_SpawnCount; i++)
		{
			SpawnRandomZombie();
		}

		m_LastSpawnTime = renderParameters.time;
		m_SpawnInterval -= 0.1f / m_SpawnCount;

		if (m_SpawnInterval < kSmallestSpawnInterval)
		{
			m_SpawnInterval *= 2;
			m_SpawnCount *= 2;
		}
	}

	if (m_Health < 1.0f)
	{
		m_Health += kHealthRegenerationRate * renderParameters.frameTime;
	}

	UpdateInput(renderParameters.frameTime);
	UpdateWeapon();
}

void PlayerInstance::RenderStatePlaying2D(RenderParameters& renderParameters)
{
	float delta = renderParameters.time - m_StartTime;
	
	auto text = "Kill count: " + to_string(m_ZombiesKilled) + "\n"
		"Number of alive zombies: " + to_string(m_Zombies.size()) + "\n"
		"You have survived for " + Tools::FloatToString(delta) + " seconds";
	renderParameters.color = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	Font::GetDefault().DrawText(text, 25, renderParameters.screenHeight - 200, renderParameters);
	
	text = "Health: " + to_string(static_cast<int>(m_Health * 100));
	renderParameters.color = DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	Font::GetDefault().DrawText(text, renderParameters.screenWidth - 250, renderParameters.screenHeight - 74, renderParameters);
}

void PlayerInstance::UpdateStateGameOver(const RenderParameters& renderParameters)
{
	auto& input = Input::GetInstance();
	
#if WINDOWS_PHONE	
	auto pinch = input.HandlePinchDisplacement();
#endif
	input.IgnoreDisplacements();

#if !WINDOWS_PHONE
	if (input.IsKeyDown(VK_RETURN))
#else
	if (pinch > 0)
#endif
	{
		auto& systemInstance = System::GetInstance();

		for (auto& zombie : m_Zombies)
		{
			systemInstance.RemoveModel(zombie.get());
		}

		m_Zombies.clear();
		StartGame();
	}
}

void PlayerInstance::RenderStateGameOver2D(RenderParameters& renderParameters)
{
	renderParameters.color = DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);

	m_BoldFont.DrawText("GAME OVER!", renderParameters.screenWidth / 2 - 350, renderParameters.screenHeight / 2 - 200, renderParameters, true);
	
	string text;
	renderParameters.color = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	int offset = 0;
	if (m_AchievedHighscore)
	{
		text = "NEW HIGHSCORE!";
		Font::GetDefault().DrawText(text, renderParameters.screenWidth / 2 - 176, renderParameters.screenHeight / 2 + 50 + offset, renderParameters);
		offset += 62;
	}
	
	text = "You have survived for " + Tools::FloatToString(m_DeathTime - m_StartTime) + " seconds";
	Font::GetDefault().DrawText(text, renderParameters.screenWidth / 2 - 355, renderParameters.screenHeight / 2 + 50 + offset, renderParameters);
	offset += 62;

	text = to_string(m_ZombiesKilled) + " zombies have fallen beneath you";
	Font::GetDefault().DrawText(text, renderParameters.screenWidth / 2 - 360, renderParameters.screenHeight / 2 + 50 + offset, renderParameters);

#if !WINDOWS_PHONE
	text = "Press enter to play again";
#else
	text = "Pinch in to play again";
#endif
	Font::GetDefault().DrawText(text, renderParameters.screenWidth / 2 - 245, renderParameters.screenHeight / 2 + 300, renderParameters, true);
	
	RenderHighscore(renderParameters);
}

void PlayerInstance::RenderHighscore(RenderParameters& renderParameters)
{
	string text;

	if (m_Highscore.GetLongestSurvivalScore() != m_Highscore.GetMostZombiesKilledScore())
	{
		text = "Longest survival highscore: survived for " + Tools::FloatToString(m_Highscore.GetLongestSurvivalScore().secondsSurvived) + " seconds" +
			", killed " + to_string(m_Highscore.GetLongestSurvivalScore().zombiesKilled) +  " zombies.\r\n" + 
			"Most zombies killed highscore: survived for " + Tools::FloatToString(m_Highscore.GetMostZombiesKilledScore().secondsSurvived) + " seconds" +
			", killed " + to_string(m_Highscore.GetMostZombiesKilledScore().zombiesKilled) +  " zombies.";

		m_SmallFont.DrawText(text, 25, renderParameters.screenHeight - 77, renderParameters);
	}
	else
	{
		text = "Highscore: survived for " + Tools::FloatToString(m_Highscore.GetLongestSurvivalScore().secondsSurvived) + " seconds" +
			", killed " + to_string(m_Highscore.GetLongestSurvivalScore().zombiesKilled) +  " zombies.";

		m_SmallFont.DrawText(text, 25, renderParameters.screenHeight - 51, renderParameters);
	}
}

void PlayerInstance::StartGame()
{
	UpdateInput(0.001f);

	for (int i = 0; i < Constants::StartingZombieCount; i++)
	{
		SpawnRandomZombie();
	}

	m_Weapon = shared_ptr<WeaponInstance>(new WeaponInstance);
	System::GetInstance().AddModel(m_Weapon);

	m_GameState = GameState::Playing;
	m_LastSpawnTime = m_StartTime = static_cast<float>(Tools::GetTime());
	m_SpawnInterval = Constants::ZombieSpawnIntervalInSeconds;
	m_SpawnCount = 1;
	m_Health = 1.0f;
	m_ZombiesKilled = 0;
}

void PlayerInstance::GameOver()
{
	System::GetInstance().RemoveModel(m_Weapon.get());
	m_Weapon = nullptr;

	m_GameState = GameState::GameOver;
	m_DeathTime = static_cast<float>(Tools::GetTime());
	m_GameOverSound.Play();
	m_AchievedHighscore = m_Highscore.SubmitScore(m_DeathTime - m_StartTime, m_ZombiesKilled);
}

void PlayerInstance::SpawnRandomZombie()
{
	auto randomValue = Tools::Random::GetNextInteger(1, 10);

	if (randomValue > 0)
	{
		SpawnZombie();
	}
	else
	{
		SpawnSuperZombie();
	}
}

void PlayerInstance::SpawnZombie()
{
	auto zombie = ZombieInstance::Spawn(*this, m_Zombies);
	
	System::GetInstance().AddModel(zombie);
	m_Zombies.push_back(zombie);
}

void PlayerInstance::SpawnSuperZombie()
{
	auto zombie = SuperZombieInstance::Spawn(*this);
	
	System::GetInstance().AddModel(zombie);
	m_Zombies.push_back(zombie);
}

void PlayerInstance::UpdateInput(float frameTime)
{
	float forward = 0.0f, right = 0.0f;
	auto& input = Input::GetInstance();

	m_CameraController.Update(frameTime, [this](const DirectX::XMFLOAT2& position) -> bool
	{
		return ZombieInstance::CanMoveTo(position, m_Zombies, nullptr);
	});
}

void PlayerInstance::UpdateWeapon()
{
	auto weaponPosition = m_CameraController.GetPosition();
	auto weaponRotation = m_CameraController.GetRotation();
	
	auto xRotationSin = sin(weaponRotation.x);
	auto xRotationCos = cos(weaponRotation.x);

	auto yRotationSin = sin(weaponRotation.y);
	auto yRotationCos = cos(weaponRotation.y);

	const auto& weaponPositionOffset = WeaponInstance::kWeaponPositionOffset;

	auto offsetZ = weaponPositionOffset.y * xRotationSin + weaponPositionOffset.z * xRotationCos;
	
	weaponPosition.x += weaponPositionOffset.x * yRotationCos + offsetZ * yRotationSin;
	weaponPosition.y += weaponPositionOffset.y * xRotationCos - weaponPositionOffset.z * xRotationSin;
	weaponPosition.z += -weaponPositionOffset.x * yRotationSin + offsetZ * yRotationCos;	

	m_Weapon->SetPosition(weaponPosition);
	m_Weapon->SetRotation(weaponRotation);

	auto& input = Input::GetInstance();

	if (input.IsMouseButtonDown(1))
	{
		m_ZombiesKilled += m_Weapon->Fire(m_Zombies, m_CameraController.GetPosition());
		input.MouseButtonUp(1);
	}
}

void PlayerInstance::TakeDamage(float damage)
{
	m_Health -= damage;

	if (m_Health <= 0.0f && m_GameState == GameState::Playing)
	{
		GameOver();
	}
}