#include "PrecompiledHeader.h"
#include "PlayerInstance.h"

#include "Constants.h"
#include "Source\Graphics\IShader.h"
#include "System.h"

PlayerInstance::PlayerInstance(const Camera& playerCamera) :
	m_Camera(playerCamera), m_LastSpawnTime(static_cast<float>(Tools::GetTime()))
{
	auto& system = System::GetInstance();

	for (int i = 0; i < Constants::StartingZombieCount; i++)
	{
		auto zombie = ZombieInstance::Spawn(*this);

		system.AddModel(zombie);
		m_Zombies.push_back(zombie);
	}
}

PlayerInstance::~PlayerInstance()
{
}

void PlayerInstance::UpdateAndRender(RenderParameters& renderParameters)
{
	
}