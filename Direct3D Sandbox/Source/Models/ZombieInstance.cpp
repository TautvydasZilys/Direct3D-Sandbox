#include "PrecompiledHeader.h"
#include "ZombieInstance.h"
#include "Source\\Graphics\\IShader.h"


ZombieInstance::ZombieInstance(const ModelParameters& modelParameters) :
	ModelInstance(IShader::GetShader(ShaderType::NORMAL_MAP_SHADER), 
					L"Assets\\Models\\Zombie.model", 
					modelParameters, 
					L"Assets\\Textures\\Zombie.dds",
					L"Assets\\Normal Maps\\Zombie.dds")
{
}

ZombieInstance::~ZombieInstance()
{
}

shared_ptr<ZombieInstance> ZombieInstance::Spawn(const DirectX::XMFLOAT3& playerPosition)
{
	static mt19937 randomEngine(static_cast<unsigned int>(Tools::GetRawTime()));
	
	uniform_real_distribution<float> radiusDistribution(5.0f, 20.0f);
	uniform_real_distribution<float> angleDistribution(0.0f, 2 * DirectX::XM_PI);
	ModelParameters modelParameters;

	auto radius = radiusDistribution(randomEngine);
	auto angle = angleDistribution(randomEngine);

	modelParameters.position = DirectX::XMFLOAT3(playerPosition.x + radius * cos(angle), 0.0f, playerPosition.z + radius * sin(angle));
	modelParameters.scale = DirectX::XMFLOAT3(0.01f, 0.01f, 0.01f);
	modelParameters.rotation = DirectX::XMFLOAT3(0.0f, -angle - DirectX::XM_PI / 2.0f, 0.0f);

	return make_shared<ZombieInstance>(modelParameters);
}