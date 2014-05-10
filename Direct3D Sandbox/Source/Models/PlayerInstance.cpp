#include "PrecompiledHeader.h"
#include "PlayerInstance.h"

#include "Constants.h"
#include "Source\Graphics\Font.h"
#include "Source\Graphics\IShader.h"
#include "System.h"

#include "SuperZombieInstance.h"
#include "WeaponInstance.h"
#include "ZombieInstance.h"

PlayerInstance::PlayerInstance(const Camera& playerCamera) :
	m_Camera(playerCamera), m_Weapon(*new WeaponInstance), m_StartTime(static_cast<float>(Tools::GetTime())), m_LastSpawnTime(m_StartTime)
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
	// Remove destroyed zombies
	for (auto i = 0u; i < m_Zombies.size(); i++)
	{
		if (m_Zombies[i].expired())
		{
			m_Zombies[i] = m_Zombies[m_Zombies.size() - 1];
			m_Zombies.pop_back();
			i--;
		}
	}

	if (renderParameters.time - m_LastSpawnTime >= Constants::ZombieSpawnIntervalInSeconds && static_cast<int>(m_Zombies.size()) < Constants::MaxZombies)
	{
		SpawnRandomZombie();
		m_LastSpawnTime = renderParameters.time;
	}
	
	UpdateWeapon();
}

void PlayerInstance::UpdateAndRender2D(RenderParameters& renderParameters)
{
	static char buffer[20];
	float delta = renderParameters.time - m_StartTime;
	sprintf_s(buffer, "%.1f", delta);

	auto text = "You have survived for " + string(buffer) + " seconds";
	Font::GetDefault().DrawText(text, 25, renderParameters.screenHeight - 75, renderParameters);
}

void PlayerInstance::SpawnRandomZombie()
{
	auto randomValue = Tools::Random::GetNextInteger(1, 10);

	if (randomValue > 1)
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
		m_Weapon.Fire();
		input.MouseButtonUp(1);
	}
}