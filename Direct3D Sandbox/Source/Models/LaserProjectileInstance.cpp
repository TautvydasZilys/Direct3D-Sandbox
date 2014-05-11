#include "PrecompiledHeader.h"

#include "LaserProjectileInstance.h"
#include "Source\Graphics\IShader.h"
#include "System.h"

static const float kRayWidth = 0.3f;
static const float kRayLifetime = 0.4f;

LaserProjectileInstance::LaserProjectileInstance(const ModelParameters& modelParameters, const DirectX::XMVECTOR& rayDirection) :
	ModelInstance3D(IShader::GetShader(ShaderType::LASER_SHADER), L"Assets\\Models\\Laser.model", modelParameters),
	m_SpawnedAt(static_cast<float>(Tools::GetTime()))
{
	m_Parameters.color = DirectX::XMFLOAT4(0.8f, 0.0f, 0.0f, 1.0f);

	DirectX::XMStoreFloat3(&m_RayDirectionNormalized, DirectX::XMVector3Normalize(rayDirection));
}

LaserProjectileInstance::~LaserProjectileInstance()
{
}

void LaserProjectileInstance::Update(const RenderParameters& renderParameters)
{
	m_TransitionProgress = (renderParameters.time - m_SpawnedAt) / kRayLifetime;

	if (m_TransitionProgress > 1.0f)
	{
		m_TransitionProgress = 1.0f;
		System::GetInstance().RemoveModel(this);
	}
}

void LaserProjectileInstance::Render3D(RenderParameters& renderParameters)
{	
	using namespace DirectX;
	
	XMVECTOR rayDirection = XMLoadFloat3(&m_RayDirectionNormalized);

	XMVECTOR viewDirection = XMLoadFloat3(&renderParameters.cameraPosition) - XMLoadFloat3(&m_Parameters.position);
	XMVECTOR rayViewDirection = XMVector3Cross(rayDirection, XMVector3Cross(rayDirection, viewDirection));

	XMStoreFloat3(&renderParameters.rayViewDirection, XMVector3Normalize(rayViewDirection));
	
	renderParameters.transitionProgress = m_TransitionProgress;
	ModelInstance3D::Render3D(renderParameters);
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
	
	shared_ptr<IModelInstance> projectile(new LaserProjectileInstance(rayParameters, rayVector));
	System::GetInstance().AddModel(projectile);
}