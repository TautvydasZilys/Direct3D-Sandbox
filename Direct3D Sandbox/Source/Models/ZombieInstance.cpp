#include "PrecompiledHeader.h"
#include "Constants.h"
#include "PlayerInstance.h"
#include "System.h"
#include "Source\\Graphics\\IShader.h"
#include "ZombieInstance.h"

const float ZombieInstance::kAnimationPeriods[ZombieStates::StateCount] = { 2.0f, 0.8f, 1.167f, 1.333f };
const bool ZombieInstance::kDoesAnimationLoop[ZombieStates::StateCount] = { true, true, true, false };
const float ZombieInstance::kAnimationTransitionLength = 0.5f;
const float ZombieInstance::kZombieDistancePerRunningAnimationTime = 1.5f;
const float ZombieInstance::kZombieBodyLastingTime = 20.0f;

ZombieInstance::ZombieInstance(const ModelParameters& modelParameters, const PlayerInstance& targetPlayer, 
							   const vector<weak_ptr<ZombieInstanceBase>>& zombies) :
	ZombieInstanceBase(IShader::GetShader(ShaderType::ANIMATION_NORMAL_MAP_SHADER), 
						L"Assets\\Animated Models\\Zombie.animatedModel", 
						L"Assets\\Textures\\Zombie.dds",
						L"Assets\\Normal Maps\\Zombie.dds",
						modelParameters,
						targetPlayer,
						kZombieDistancePerRunningAnimationTime / kAnimationPeriods[ZombieStates::Running]),
	m_AnimationStateMachine(ZombieStates::Idle),
	m_Zombies(zombies),
	m_GonnaLiveFor(Tools::Random::GetNextReal<float>(5.0f, 15.0f))
{
	for (int i = 0; i < ZombieStates::Death; i++)
	{
		m_AnimationStateMachine.SetAnimationProgress(i, Tools::Random::GetNextReal(0.0f, 1.0f));
	}

	m_AnimationStateMachine.SetAnimationProgress(ZombieStates::Death, 0.145f);
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

				if (!zombie->IsDead() && distanceSqr < 1.0f)
				{
					return false;
				}
			}
		}
	}
	
	return true;
}

void ZombieInstance::UpdateAndRender3D(RenderParameters& renderParameters)
{	
	ZombieStates targetState;

	if (!m_IsDead)
	{
		auto playerPosition = m_TargetPlayer.GetPosition();
		auto angleY = -atan2(m_Parameters.position.z - playerPosition.z, m_Parameters.position.x - playerPosition.x) - DirectX::XM_PI / 2.0f;

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

		if (renderParameters.time - m_SpawnTime > m_GonnaLiveFor)
		{
			m_IsDead = true;
		}

		SetRotation(DirectX::XMFLOAT3(0.0f, angleY, 0.0f));
	}
	else
	{
		targetState = ZombieStates::Death;

		if (renderParameters.time - m_SpawnTime - m_GonnaLiveFor > kZombieBodyLastingTime)
		{
			System::GetInstance().RemoveModel(this);
		}
	}

	m_AnimationStateMachine.Update(renderParameters, targetState);

	ZombieInstanceBase::UpdateAndRender3D(renderParameters);
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