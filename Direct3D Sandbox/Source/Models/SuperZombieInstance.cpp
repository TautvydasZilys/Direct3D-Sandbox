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
						targetPlayer,
						0.0f)
{
}

SuperZombieInstance::~SuperZombieInstance()
{
}

void SuperZombieInstance::UpdateAndRender3D(RenderParameters& renderParameters)
{
	ZombieInstanceBase::UpdateAndRender3D(renderParameters);
}

shared_ptr<ZombieInstanceBase> SuperZombieInstance::Spawn(const PlayerInstance& targetPlayer)
{
	return shared_ptr<ZombieInstanceBase>(new SuperZombieInstance(GetRandomZombieParameters(targetPlayer), targetPlayer));
}