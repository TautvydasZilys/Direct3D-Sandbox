#include "PrecompiledHeader.h"
#include "Constants.h"
#include "PlayerInstance.h"
#include "Source\\Graphics\\IShader.h"
#include "ZombieInstanceBase.h"


ZombieInstanceBase::ZombieInstanceBase(IShader& shader, const wstring& modelPath, const wstring& texturePath, const wstring& normalMapPath,
		const ModelParameters& modelParameters, const PlayerInstance& targetPlayer) :
	ModelInstance(shader, modelPath, modelParameters, texturePath, normalMapPath),
	m_TargetPlayer(targetPlayer)
{
}

ZombieInstanceBase::~ZombieInstanceBase()
{
}

void ZombieInstanceBase::UpdateAndRender(RenderParameters& renderParameters)
{
#if !DISABLE_ZOMBIE_MOVEMENT
	auto playerPosition = m_TargetPlayer.GetPosition();
	auto angleY = -atan2(m_Parameters.position.z - playerPosition.z, m_Parameters.position.x - playerPosition.x) - DirectX::XM_PI / 2.0f;

	DirectX::XMFLOAT2 vectorToPlayer(playerPosition.x - m_Parameters.position.x, playerPosition.z - m_Parameters.position.z);
	DirectX::XMFLOAT2 zombieMovementThisTurn;
	auto distanceToPlayerSqr = vectorToPlayer.x * vectorToPlayer.x + vectorToPlayer.y * vectorToPlayer.y;
	
	if (distanceToPlayerSqr > 0.25f)
	{
		auto vectorMultiplier = Constants::ZombieSpeed * renderParameters.frameTime / sqrt(distanceToPlayerSqr);

		m_Parameters.position.x += vectorMultiplier * vectorToPlayer.x;
		m_Parameters.position.z += vectorMultiplier * vectorToPlayer.y;
	}

	SetRotation(DirectX::XMFLOAT3(0.0f, angleY, 0.0f));
#endif

	ModelInstance::UpdateAndRender(renderParameters);
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