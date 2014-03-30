#include "PrecompiledHeader.h"
#include "PlayerInstance.h"

#include "Constants.h"
#include "Source\Graphics\IShader.h"
#include "System.h"

PlayerInstance::PlayerInstance(const Camera& playerCamera) :
	m_Camera(playerCamera), m_LastSpawnTime(static_cast<float>(Tools::GetTime()))
{
	for (int i = 0; i < Constants::StartingZombieCount; i++)
	{
		SpawnZombie();
	}
}

PlayerInstance::~PlayerInstance()
{
}

void PlayerInstance::UpdateAndRender(RenderParameters& renderParameters)
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

	if (renderParameters.time - m_LastSpawnTime >= Constants::ZombieSpawnIntervalInSeconds && m_Zombies.size() < Constants::MaxZombies)
	{
		SpawnZombie();
		m_LastSpawnTime = renderParameters.time;
	}
}

void PlayerInstance::SpawnZombie()
{
	auto zombie = ZombieInstance::Spawn(*this);
	
	System::GetInstance().AddModel(zombie);
	m_Zombies.push_back(zombie);
}