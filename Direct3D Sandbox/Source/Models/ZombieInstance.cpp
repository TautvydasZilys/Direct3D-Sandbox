#include "PrecompiledHeader.h"
#include "Constants.h"
#include "PlayerInstance.h"
#include "Source\\Graphics\\IShader.h"
#include "ZombieInstance.h"

const float ZombieInstance::kIdleAnimationPeriod = 2.0f;
const float ZombieInstance::kRunningAnimationPeriod = 0.75f;
const float ZombieInstance::kZombieDistancePerAnimationTime = 1.5f;

ZombieInstance::ZombieInstance(const ModelParameters& modelParameters, const PlayerInstance& targetPlayer, 
							   const vector<weak_ptr<ZombieInstanceBase>>& zombies) :
	ZombieInstanceBase(IShader::GetShader(ShaderType::ANIMATION_NORMAL_MAP_SHADER), 
						L"Assets\\Animated Models\\Zombie.animatedModel", 
						L"Assets\\Textures\\Zombie.dds",
						L"Assets\\Normal Maps\\Zombie.dds",
						modelParameters,
						targetPlayer,
						kZombieDistancePerAnimationTime / kRunningAnimationPeriod),
	m_AnimationProgress(Tools::Random::GetNextReal(0.0f, 1.0f)),
	m_Zombies(zombies)
{
}

ZombieInstance::~ZombieInstance()
{
}

bool ZombieInstance::CanMoveTo(const DirectX::XMFLOAT2& position, const vector<weak_ptr<ZombieInstanceBase>>& zombies,
	const ZombieInstanceBase* thisPtr)
{
	auto zombieCount = zombies.size();

	for (auto i = 0u; i < zombieCount; i++)
	{
		if (!zombies[i].expired())
		{
			auto zombie = zombies[i].lock().get();

			if (zombie != thisPtr)
			{
				auto distanceSqr = zombie->HorizontalDistanceSqrTo(position);

				if (distanceSqr < 1.0f)
				{
					return false;
				}
			}
		}
	}
	
	return true;
}

void ZombieInstance::UpdateAndRender(RenderParameters& renderParameters)
{	
#if !DISABLE_ZOMBIE_MOVEMENT
	auto playerPosition = m_TargetPlayer.GetPosition();
	auto angleY = -atan2(m_Parameters.position.z - playerPosition.z, m_Parameters.position.x - playerPosition.x) - DirectX::XM_PI / 2.0f;

	DirectX::XMFLOAT2 vectorToPlayer(playerPosition.x - m_Parameters.position.x, playerPosition.z - m_Parameters.position.z);
	DirectX::XMFLOAT2 zombieMovementThisTurn;
	auto distanceToPlayerSqr = vectorToPlayer.x * vectorToPlayer.x + vectorToPlayer.y * vectorToPlayer.y;
	
	if (distanceToPlayerSqr > 0.25f)
	{
		auto vectorMultiplier = m_Speed * renderParameters.frameTime / sqrt(distanceToPlayerSqr);

		DirectX::XMFLOAT2 newPosition(m_Parameters.position.x + vectorMultiplier * vectorToPlayer.x, 
									  m_Parameters.position.z + vectorMultiplier * vectorToPlayer.y);

		if (CanMoveTo(newPosition, m_Zombies, this))
		{
			m_AnimationProgress += renderParameters.frameTime / kRunningAnimationPeriod;
			renderParameters.currentAnimationState = ZombieStates::Running;

			m_Parameters.position.x = newPosition.x;
			m_Parameters.position.z = newPosition.y;
		}
		else
		{
			m_AnimationProgress += renderParameters.frameTime / kIdleAnimationPeriod;
			renderParameters.currentAnimationState = ZombieStates::Idle;
		}
	}
	
	SetRotation(DirectX::XMFLOAT3(0.0f, angleY, 0.0f));
	renderParameters.animationProgress = m_AnimationProgress - floor(m_AnimationProgress);
#endif

	ZombieInstanceBase::UpdateAndRender(renderParameters);
}

shared_ptr<ZombieInstanceBase> ZombieInstance::Spawn(const PlayerInstance& targetPlayer, const vector<weak_ptr<ZombieInstanceBase>>& zombies)
{
	ModelParameters zombieParameters;

	do
	{
		zombieParameters = GetRandomZombieParameters(targetPlayer);
	}
	while (!CanMoveTo(DirectX::XMFLOAT2(zombieParameters.position.x, zombieParameters.position.z), zombies, nullptr));

	return shared_ptr<ZombieInstanceBase>(new ZombieInstance(zombieParameters, targetPlayer, zombies));
}