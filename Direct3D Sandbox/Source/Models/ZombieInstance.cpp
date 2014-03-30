#include "PrecompiledHeader.h"
#include "Constants.h"
#include "PlayerInstance.h"
#include "Source\\Graphics\\IShader.h"
#include "ZombieInstance.h"


ZombieInstance::ZombieInstance(const ModelParameters& modelParameters, const PlayerInstance& targetPlayer) :
	ModelInstance(IShader::GetShader(ShaderType::NORMAL_MAP_SHADER), 
					L"Assets\\Models\\Zombie.model", 
					modelParameters, 
					L"Assets\\Textures\\Zombie.dds",
					L"Assets\\Normal Maps\\Zombie.dds"),
	m_TargetPlayer(targetPlayer)
{
}

ZombieInstance::~ZombieInstance()
{
}

void ZombieInstance::UpdateAndRender(RenderParameters& renderParameters)
{
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
	ModelInstance::UpdateAndRender(renderParameters);
}

shared_ptr<ZombieInstance> ZombieInstance::Spawn(const PlayerInstance& targetPlayer)
{
	static mt19937 randomEngine(static_cast<unsigned int>(Tools::GetRawTime()));
	
	uniform_real_distribution<float> radiusDistribution(5.0f, 20.0f);
	uniform_real_distribution<float> angleDistribution(0.0f, 2 * DirectX::XM_PI);
	ModelParameters modelParameters;

	auto radius = radiusDistribution(randomEngine);
	auto angle = angleDistribution(randomEngine);
	auto playerPosition = targetPlayer.GetPosition();

	modelParameters.position = DirectX::XMFLOAT3(playerPosition.x + radius * cos(angle), 0.0f, playerPosition.z + radius * sin(angle));
	modelParameters.scale = DirectX::XMFLOAT3(0.01f, 0.01f, 0.01f);
	modelParameters.rotation = DirectX::XMFLOAT3(0.0f, -angle - DirectX::XM_PI / 2.0f, 0.0f);

	return make_shared<ZombieInstance>(modelParameters, targetPlayer);
}