#include "PrecompiledHeader.h"
#include "Constants.h"
#include "PlayerInstance.h"
#include "Source\\Graphics\\IShader.h"
#include "ZombieInstanceBase.h"


ZombieInstanceBase::ZombieInstanceBase(IShader& shader, const wstring& modelPath, const wstring& texturePath, const wstring& normalMapPath,
		const ModelParameters& modelParameters, const PlayerInstance& targetPlayer, float speed) :
	ModelInstance(shader, modelPath, modelParameters, texturePath, normalMapPath),
	m_TargetPlayer(targetPlayer),
	m_Speed(speed),
	m_IsDead(false),
	m_SpawnTime(static_cast<float>(Tools::GetTime()))
{
}

ZombieInstanceBase::~ZombieInstanceBase()
{
}

void ZombieInstanceBase::UpdateAndRender(RenderParameters& renderParameters)
{
	ModelInstance::UpdateAndRender(renderParameters);
}

ModelParameters ZombieInstanceBase::GetRandomZombieParameters(const PlayerInstance& targetPlayer)
{
	ModelParameters modelParameters;

	auto radius = Tools::Random::GetNextReal(5.0f, 20.0f);
	auto angle = Tools::Random::GetNextReal(0.0f, 2 * DirectX::XM_PI);
	auto playerPosition = targetPlayer.GetPosition();

	modelParameters.position = DirectX::XMFLOAT3(playerPosition.x + radius * cos(angle), 0.0f, playerPosition.z + radius * sin(angle));
	modelParameters.scale = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);
	modelParameters.rotation = DirectX::XMFLOAT3(0.0f, -angle - DirectX::XM_PI / 2.0f, 0.0f);

	return modelParameters;
}