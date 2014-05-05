#include "PrecompiledHeader.h"
#include "Constants.h"
#include "PlayerInstance.h"
#include "Source\\Graphics\\IShader.h"
#include "ZombieInstance.h"

const float ZombieInstance::kAnimationPeriods[ZombieStates::StateCount] = { 2.0f, 0.8f, 1.167f };
const float ZombieInstance::kZombieDistancePerRunningAnimationTime = 1.5f;

ZombieInstance::ZombieInstance(const ModelParameters& modelParameters, const PlayerInstance& targetPlayer, 
							   const vector<weak_ptr<ZombieInstanceBase>>& zombies) :
	ZombieInstanceBase(IShader::GetShader(ShaderType::ANIMATION_NORMAL_MAP_SHADER), 
						L"Assets\\Animated Models\\Zombie.animatedModel", 
						L"Assets\\Textures\\Zombie.dds",
						L"Assets\\Normal Maps\\Zombie.dds",
						modelParameters,
						targetPlayer,
						kZombieDistancePerRunningAnimationTime / kAnimationPeriods[ZombieStates::Running]),
	m_CurrentState(ZombieStates::Idle),
	m_IsTransitioningAnimationStates(false),
	m_Zombies(zombies)
{
	for (int i = 0; i < ZombieStates::StateCount; i++)
	{
		m_AnimationProgress[i] = Tools::Random::GetNextReal(0.0f, 1.0f);
	}
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

float ZombieInstance::GetAnimationTransitionLength(ZombieStates from, ZombieStates to)
{
	return 0.5f;
}

void ZombieInstance::SetAnimationParameters(RenderParameters& renderParameters, ZombieStates targetState)
{
	if (m_CurrentState != targetState || m_TransitionProgress > 0.0f)
	{
		if (!m_IsTransitioningAnimationStates)
		{
			m_IsTransitioningAnimationStates = true;
			m_TransitionProgress = 0.0f;
		}
		else
		{
			if (m_CurrentState == targetState)
			{
				targetState = m_CurrentState;
				m_CurrentState = static_cast<ZombieStates>(abs(m_CurrentState - 1));
				m_TransitionProgress = 1.0f - m_TransitionProgress;
			}

			m_TransitionProgress += renderParameters.frameTime / GetAnimationTransitionLength(m_CurrentState, targetState);

			if (m_TransitionProgress >= 1.0f)
			{
				m_IsTransitioningAnimationStates = false;
				m_TransitionProgress = 0.0f;
				m_CurrentState = targetState;
			}
		}
		
		renderParameters.targetAnimationState = targetState;
		renderParameters.targetStateAnimationProgress = m_AnimationProgress[targetState] - floor(m_AnimationProgress[targetState]);
		renderParameters.transitionProgress = m_TransitionProgress;
	}
	else
	{
		m_IsTransitioningAnimationStates = false;
		m_AnimationProgress[m_CurrentState] += renderParameters.frameTime / kAnimationPeriods[m_CurrentState];
	}

	renderParameters.isTransitioningAnimationStates = m_IsTransitioningAnimationStates;
	renderParameters.currentStateAnimationProgress = m_AnimationProgress[m_CurrentState] - floor(m_AnimationProgress[m_CurrentState]);
	renderParameters.currentAnimationState = m_CurrentState;
}

void ZombieInstance::UpdateAndRender(RenderParameters& renderParameters)
{	
#if !DISABLE_ZOMBIE_MOVEMENT
	auto playerPosition = m_TargetPlayer.GetPosition();
	auto angleY = -atan2(m_Parameters.position.z - playerPosition.z, m_Parameters.position.x - playerPosition.x) - DirectX::XM_PI / 2.0f;
	
	ZombieStates targetState;

	DirectX::XMFLOAT2 vectorToPlayer(playerPosition.x - m_Parameters.position.x, playerPosition.z - m_Parameters.position.z);
	auto distanceToPlayerSqr = vectorToPlayer.x * vectorToPlayer.x + vectorToPlayer.y * vectorToPlayer.y;	

	if (distanceToPlayerSqr > 1.5f)
	{
		auto vectorMultiplier = m_Speed * renderParameters.frameTime / sqrt(distanceToPlayerSqr);

		DirectX::XMFLOAT2 newPosition(m_Parameters.position.x + vectorMultiplier * vectorToPlayer.x, 
									  m_Parameters.position.z + vectorMultiplier * vectorToPlayer.y);

		if (CanMoveTo(newPosition, m_Zombies, this))
		{
			targetState = ZombieStates::Running;

			m_Parameters.position.x = newPosition.x;
			m_Parameters.position.z = newPosition.y;
		}
		else
		{
			targetState = ZombieStates::Idle;
		}
	}
	else
	{
		targetState = ZombieStates::Hitting;
	}

	SetRotation(DirectX::XMFLOAT3(0.0f, angleY, 0.0f));

	SetAnimationParameters(renderParameters, targetState);
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