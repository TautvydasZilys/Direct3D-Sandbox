#include "PrecompiledHeader.h"
#include "Constants.h"
#include "PlayerInstance.h"
#include "Source\\Graphics\\IShader.h"
#include "ZombieInstance.h"

const float ZombieInstance::kAnimationPeriod = 3.0f;
const float ZombieInstance::kZombieDistancePerAnimationTime = 1.5f;

ZombieInstance::ZombieInstance(const ModelParameters& modelParameters, const PlayerInstance& targetPlayer) :
	ZombieInstanceBase(IShader::GetShader(ShaderType::ANIMATION_NORMAL_MAP_SHADER), 
						L"Assets\\Animated Models\\Zombie.animatedModel", 
						L"Assets\\Textures\\Zombie.dds",
						L"Assets\\Normal Maps\\Zombie.dds",
						modelParameters,
						targetPlayer,
						kZombieDistancePerAnimationTime / kAnimationPeriod),
	m_StartingAnimationProgress(Tools::Random::GetNextReal(0.0f, 1.0f))
{
}

ZombieInstance::~ZombieInstance()
{
}

void ZombieInstance::UpdateAndRender(RenderParameters& renderParameters)
{
	renderParameters.animationProgress = renderParameters.time / kAnimationPeriod + m_StartingAnimationProgress;
	renderParameters.animationProgress = renderParameters.animationProgress - floor(renderParameters.animationProgress);

	ZombieInstanceBase::UpdateAndRender(renderParameters);
}

shared_ptr<ZombieInstanceBase> ZombieInstance::Spawn(const PlayerInstance& targetPlayer)
{
	return shared_ptr<ZombieInstanceBase>(new ZombieInstance(GetRandomZombieParameters(targetPlayer), targetPlayer));
}