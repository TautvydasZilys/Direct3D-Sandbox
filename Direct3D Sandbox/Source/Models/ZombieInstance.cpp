#include "PrecompiledHeader.h"
#include "Constants.h"
#include "PlayerInstance.h"
#include "Source\\Graphics\\IShader.h"
#include "ZombieInstance.h"


ZombieInstance::ZombieInstance(const ModelParameters& modelParameters, const PlayerInstance& targetPlayer) :
	ZombieInstanceBase(IShader::GetShader(ShaderType::NORMAL_MAP_SHADER), 
						L"Assets\\Models\\Zombie.model", 
						L"Assets\\Textures\\Zombie.dds",
						L"Assets\\Normal Maps\\Zombie.dds",
						modelParameters,
						targetPlayer)
{
}

ZombieInstance::~ZombieInstance()
{
}

void ZombieInstance::UpdateAndRender(RenderParameters& renderParameters)
{
	ZombieInstanceBase::UpdateAndRender(renderParameters);
}

shared_ptr<ZombieInstanceBase> ZombieInstance::Spawn(const PlayerInstance& targetPlayer)
{
	return make_shared<ZombieInstance>(GetRandomZombieParameters(targetPlayer), targetPlayer);
}