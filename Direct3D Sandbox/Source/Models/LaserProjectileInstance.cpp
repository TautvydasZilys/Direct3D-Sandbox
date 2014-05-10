#include "PrecompiledHeader.h"

#include "LaserProjectileInstance.h"
#include "Source\Graphics\IShader.h"
#include "System.h"

static const float kRayWidth = 0.1f;
static const float kRayLifetime = 0.2f;

LaserProjectileInstance::LaserProjectileInstance(const ModelParameters& modelParameters) :
	ModelInstance3D(IShader::GetShader(ShaderType::COLOR_SHADER), L"Assets\\Models\\Cylinder.model", modelParameters),
	m_SpawnedAt(static_cast<float>(Tools::GetTime()))
{
	m_Parameters.color = DirectX::XMFLOAT4(0.8f, 0.0f, 0.0f, 1.0f);
}

LaserProjectileInstance::~LaserProjectileInstance()
{
}

void LaserProjectileInstance::UpdateAndRender3D(RenderParameters& renderParameters)
{
	if (renderParameters.time - m_SpawnedAt > kRayLifetime)
	{
		System::GetInstance().RemoveModel(this);
		return;
	}
	else
	{
		ModelInstance3D::UpdateAndRender3D(renderParameters);
	}
}

void LaserProjectileInstance::Spawn(const DirectX::XMVECTOR& source, const DirectX::XMVECTOR& target)
{
	using namespace DirectX;
	
	ModelParameters rayParameters;
	
	XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMVECTOR rayVector = target - source;
	
	rayParameters.scale.x = kRayWidth;
	XMStoreFloat(&rayParameters.scale.y, XMVector3Length(rayVector));
	rayParameters.scale.z = kRayWidth;
	
	XMStoreFloat(&rayParameters.rotation.x, XMVector3AngleBetweenVectors(up, rayVector));
	rayParameters.rotation.y = atan2(XMVectorGetX(rayVector), XMVectorGetZ(rayVector));
	
	XMStoreFloat3(&rayParameters.position, (source + target) / 2.0f);
	
	shared_ptr<IModelInstance> projectile(new LaserProjectileInstance(rayParameters));
	System::GetInstance().AddModel(projectile);
}