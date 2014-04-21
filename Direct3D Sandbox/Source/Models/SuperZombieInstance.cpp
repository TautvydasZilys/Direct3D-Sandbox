#include "PrecompiledHeader.h"
#include "Constants.h"
#include "PlayerInstance.h"
#include "Source\\Graphics\\IShader.h"
#include "SuperZombieInstance.h"


SuperZombieInstance::SuperZombieInstance(const ModelParameters& modelParameters, const PlayerInstance& targetPlayer) :
	ZombieInstanceBase(IShader::GetShader(ShaderType::LIGHTING_SHADER), 
						L"Assets\\Models\\SuperZombie.model",
						L"Assets\\Textures\\SuperZombie.dds",
						L"Assets\\Normal Maps\\SuperZombie.dds", 
						modelParameters, 
						targetPlayer)
{
}

SuperZombieInstance::~SuperZombieInstance()
{
}

void SuperZombieInstance::UpdateAndRender(RenderParameters& renderParameters)
{
	ZombieInstanceBase::UpdateAndRender(renderParameters);
}

shared_ptr<ZombieInstanceBase> SuperZombieInstance::Spawn(const PlayerInstance& targetPlayer)
{
	return make_shared<SuperZombieInstance>(GetRandomZombieParameters(targetPlayer), targetPlayer);
}