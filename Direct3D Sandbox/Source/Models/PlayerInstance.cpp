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

PlayerInstance::PlayerInstance(Camera& playerCamera) :
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
		System::GetInstance().RemoveModel(zombie.get());
	}

	System::GetInstance().RemoveModel(&m_Weapon);
}

void PlayerInstance::UpdateAndRender3D(RenderParameters& renderParameters)
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

	UpdateInput(renderParameters.frameTime);
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

void PlayerInstance::UpdateInput(float frameTime)
{
	float forward = 0.0f, right = 0.0f;
	auto& input = Input::GetInstance();

	forward += input.HandlePinchDisplacement() / 100.0f;

	if (input.IsKeyDown(VK_OEM_3))
	{
		forward += 100.0f * frameTime;
	}

	if (input.IsKeyDown('W'))
	{
		forward += 5.0f * frameTime;
	}
	if (input.IsKeyDown('S'))
	{
		forward -= 5.0f * frameTime;
	}
	if (input.IsKeyDown('A'))
	{
		right -= 5.0f * frameTime;
	}
	if (input.IsKeyDown('D'))
	{
		right += 5.0f * frameTime;
	}

	if (forward != 0.0f && right != 0.0f)
	{
		forward /= sqrt(2.0f);
		right /= sqrt(2.0f);
	}
	
	auto& rotation = m_Camera.GetRotation();
	auto position = m_Camera.GetPosition();
	
	position.x += -forward * sin(rotation.y) + right * cos(rotation.y);
	position.z -= forward * cos(rotation.y) + right * sin(rotation.y);

	if (ZombieInstance::CanMoveTo(DirectX::XMFLOAT2(position.x, position.z), m_Zombies, nullptr))
	{
		m_Camera.SetPosition(position);
	}

	if (input.IsKeyDown(VK_SPACE))
	{
		m_Camera.GoUp(5.0f * frameTime);
	}
	if (input.IsKeyDown('X'))
	{
		m_Camera.GoDown(5.0f * frameTime);
	}
	
	if (input.IsKeyDown(VK_UP))
	{
		m_Camera.LookUp(frameTime / 2.0f);
	}
	if (input.IsKeyDown(VK_DOWN))
	{
		m_Camera.LookDown(frameTime / 2.0f);
	}
	if (input.IsKeyDown(VK_LEFT))
	{
		m_Camera.LookLeft(frameTime / 2.0f);
	}
	if (input.IsKeyDown(VK_RIGHT))
	{
		m_Camera.LookRight(frameTime / 2.0f);
	}
	
	long mouseX, mouseY;
	auto mouseSensitivity = System::GetInstance().GetMouseSensitivity();

	input.HandleMouseDisplacement(mouseX, mouseY);

	if (mouseX > 0.000001f || mouseX < -0.000001f)
	{
		m_Camera.LookRight(mouseSensitivity * mouseX / 250.0f);
	}

	if (mouseY > 0.000001f || mouseY < -0.000001f)
	{
		m_Camera.LookDown(mouseSensitivity * mouseY / 250.0f);
	}
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