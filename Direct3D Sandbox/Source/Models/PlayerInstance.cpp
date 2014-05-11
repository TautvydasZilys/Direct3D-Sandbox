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

PlayerInstance::PlayerInstance(const Camera& playerCamera) :
	m_Camera(playerCamera),
	m_Weapon(*new WeaponInstance), 
	m_StartTime(static_cast<float>(Tools::GetTime())), 
	m_LastSpawnTime(m_StartTime),
	m_SpawnInterval(Constants::ZombieSpawnIntervalInSeconds),
	m_SpawnCount(1),
	m_ZombiesKilled(0)
{
	for (int i = 0; i < Constants::StartingZombieCount; i++)
	{
		SpawnRandomZombie();
	}

	System::GetInstance().AddModel(shared_ptr<WeaponInstance>(&m_Weapon));
}

PlayerInstance::~PlayerInstance()
{
	for (auto& zombie : m_Zombies)
	{
		if (!zombie.expired())
		{
			System::GetInstance().RemoveModel(zombie.lock().get());
		}
	}

	System::GetInstance().RemoveModel(&m_Weapon);
}

void PlayerInstance::UpdateAndRender3D(RenderParameters& renderParameters)
{
	// Remove destroyed/dead zombies
	for (auto i = 0u; i < m_Zombies.size(); i++)
	{
		if (m_Zombies[i].expired() || m_Zombies[i].lock()->IsDead())
		{
			m_Zombies[i] = m_Zombies[m_Zombies.size() - 1];
			m_Zombies.pop_back();
			i--;
		}
	}

	if (renderParameters.time - m_LastSpawnTime >= m_SpawnInterval)// && static_cast<int>(m_Zombies.size()) < Constants::MaxZombies)
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
			m_SpawnCount++;
		}
	}
	
	UpdateWeapon();
}

void PlayerInstance::UpdateAndRender2D(RenderParameters& renderParameters)
{
	static char buffer[20];
	float delta = renderParameters.time - m_StartTime;
	sprintf_s(buffer, "%.1f", delta);
	
	auto text = "Kill count: " + to_string(m_ZombiesKilled) + "\n"
		"Number of alive zombies: " + to_string(m_Zombies.size()) + "\n"
		"You have survived for " + string(buffer) + " seconds";
	Font::GetDefault().DrawText(text, 25, renderParameters.screenHeight - 200, renderParameters);
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

void PlayerInstance::UpdateWeapon()
{
	auto weaponPosition = m_Camera.GetPosition();
	auto weaponRotation = m_Camera.GetRotation();
	
	auto xRotationSin = sin(weaponRotation.x);
	auto xRotationCos = cos(weaponRotation.x);

	auto yRotationSin = sin(weaponRotation.y);
	auto yRotationCos = cos(weaponRotation.y);

	const auto& weaponPositionOffset = WeaponInstance::kWeaponPositionOffset;

	auto offsetZ = weaponPositionOffset.y * xRotationSin + weaponPositionOffset.z * xRotationCos;
	
	weaponPosition.x += weaponPositionOffset.x * yRotationCos + offsetZ * yRotationSin;
	weaponPosition.y += weaponPositionOffset.y * xRotationCos - weaponPositionOffset.z * xRotationSin;
	weaponPosition.z += -weaponPositionOffset.x * yRotationSin + offsetZ * yRotationCos;	

	m_Weapon.SetPosition(weaponPosition);
	m_Weapon.SetRotation(weaponRotation);

	auto& input = Input::GetInstance();

	if (input.IsMouseButtonDown(1))
	{
		m_ZombiesKilled += m_Weapon.Fire(m_Zombies, m_Camera.GetPosition());
		input.MouseButtonUp(1);
	}
}