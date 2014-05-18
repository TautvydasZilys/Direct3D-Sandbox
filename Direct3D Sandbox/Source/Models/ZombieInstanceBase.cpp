#include "PrecompiledHeader.h"
#include "Constants.h"
#include "PlayerInstance.h"
#include "Source\Audio\AudioManager.h"
#include "Source\Graphics\IShader.h"
#include "ZombieInstanceBase.h"


ZombieInstanceBase::ZombieInstanceBase(IShader& shader, const wstring& modelPath, const wstring& texturePath, const wstring& normalMapPath,
		const ModelParameters& modelParameters, PlayerInstance& targetPlayer, float speed) :
	ModelInstance3D(shader, modelPath, modelParameters, texturePath, normalMapPath),
	m_TargetPlayer(targetPlayer),
	m_Speed(speed),
	m_IsDead(false),
	m_Health(1.0f),
	m_SpawnTime(static_cast<float>(Tools::GetTime())),
	m_AudioEmitter(0.5f),
	m_DeathSound(AudioManager::GetCachedSound(L"Assets\\Sounds\\ZombieDeath.wav", false, true))
{
}

ZombieInstanceBase::~ZombieInstanceBase()
{
}

void ZombieInstanceBase::Render3D(RenderParameters& renderParameters)
{
	ModelInstance3D::Render3D(renderParameters);
}

// Returns whether the zombie is dead after taking damage
bool ZombieInstanceBase::TakeDamage(float damage)
{
	m_Health -= damage;

	if (m_Health <= 0.0f)
	{
		m_DeathTime = static_cast<float>(Tools::GetTime());
		m_IsDead = true;
		m_DeathSound.Play3D(m_AudioEmitter);
	}

	return m_IsDead;
}

ModelParameters ZombieInstanceBase::GetRandomZombieParameters(const PlayerInstance& targetPlayer)
{
	ModelParameters modelParameters;

	auto radius = Tools::Random::GetNextReal(5.0f, 20.0f);
	auto angle = Tools::Random::GetNextReal(0.0f, 2 * DirectX::XM_PI);
	auto playerPosition = targetPlayer.GetPosition();

	modelParameters.position = DirectX::XMFLOAT3(playerPosition.x + radius * cos(angle), 0.0f, playerPosition.z + radius * sin(angle));
	modelParameters.scale = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);
	modelParameters.rotation = DirectX::XMFLOAT3(0.0f, -angle - DirectX::XM_PI / 2.0f, 0.0f);

	return modelParameters;
}