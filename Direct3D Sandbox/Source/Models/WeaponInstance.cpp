#include "PrecompiledHeader.h"

#include "Source\Graphics\IShader.h"
#include "Source\Graphics\Texture.h"
#include "LaserProjectileInstance.h"
#include "ModelInstance2D.h"
#include "System.h"
#include "WeaponInstance.h"

const DirectX::XMFLOAT3 WeaponInstance::kWeaponPositionOffset(0.15f, -0.2f, -0.5f);

WeaponInstance::WeaponInstance() :	
	ModelInstance3D(IShader::GetShader(ShaderType::LIGHTING_SHADER), L"Assets\\Models\\Weapon.model", ModelParameters(), L"Assets\\Textures\\Weapon.dds"),
	m_Crosshair(*new ModelInstance2D(IShader::GetShader(ShaderType::TEXTURE_SHADER),
									L"Assets\\Models\\Square.model", 
									ModelParameters(), 
									L"Assets\\Textures\\Crosshair.dds"))
{
	m_Crosshair.SetScale(DirectX::XMFLOAT3(50.0f, 50.0f, 50.0f));
	System::GetInstance().AddModel(shared_ptr<IModelInstance>(&m_Crosshair));
}

WeaponInstance::~WeaponInstance()
{
	System::GetInstance().RemoveModel(&m_Crosshair);
}

void WeaponInstance::Fire()
{
	using namespace DirectX;

	XMMATRIX rotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(m_Parameters.rotation.x, m_Parameters.rotation.y, m_Parameters.rotation.z);

	XMVECTOR sourceDelta = XMVector3Transform(XMVectorSet(0.0f, 0.02f, -0.5f, 1.0f), rotationMatrix);
	XMVECTOR targetDelta = XMVector3Transform(XMVectorSet(0.0f, -2 * kWeaponPositionOffset.y, -100.0f, 1.0f), rotationMatrix);

	XMVECTOR source = XMLoadFloat3(&m_Parameters.position) + sourceDelta;

	LaserProjectileInstance::Spawn(source, source + targetDelta);
}