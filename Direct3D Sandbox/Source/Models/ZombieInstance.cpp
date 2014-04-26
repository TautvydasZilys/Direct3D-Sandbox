#include "PrecompiledHeader.h"
#include "Constants.h"
#include "PlayerInstance.h"
#include "Source\\Graphics\\IShader.h"
#include "ZombieInstance.h"

const float ZombieInstance::s_AnimationPeriod = 3.0f;

ZombieInstance::ZombieInstance(const ModelParameters& modelParameters, const PlayerInstance& targetPlayer) :
	ZombieInstanceBase(IShader::GetShader(ShaderType::NORMAL_MAP_SHADER), 
						L"Assets\\Animated Models\\Zombie.animatedModel", 
						L"Assets\\Textures\\Zombie.dds",
						L"Assets\\Normal Maps\\Zombie.dds",
						modelParameters,
						targetPlayer),
	m_StartingAnimationProgress(Tools::Random::GetNextReal(0.0f, 1.0f))
{
}

ZombieInstance::~ZombieInstance()
{
}

void ZombieInstance::UpdateAndRender(RenderParameters& renderParameters)
{
	renderParameters.animationProgress = renderParameters.time / s_AnimationPeriod + m_StartingAnimationProgress;
	renderParameters.animationProgress = renderParameters.animationProgress - floor(renderParameters.animationProgress);

	ZombieInstanceBase::UpdateAndRender(renderParameters);
}

shared_ptr<ZombieInstanceBase> ZombieInstance::Spawn(const PlayerInstance& targetPlayer)
{
	return make_shared<ZombieInstance>(GetRandomZombieParameters(targetPlayer), targetPlayer);
}