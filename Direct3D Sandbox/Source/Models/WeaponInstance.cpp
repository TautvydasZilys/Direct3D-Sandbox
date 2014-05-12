#include "PrecompiledHeader.h"

#include "Source\Graphics\IShader.h"
#include "Source\Graphics\Texture.h"
#include "LaserProjectileInstance.h"
#include "ModelInstance2D.h"
#include "System.h"
#include "WeaponInstance.h"
#include "ZombieInstanceBase.h"

const DirectX::XMFLOAT3 WeaponInstance::kWeaponPositionOffset(0.15f, -0.2f, -0.5f);
static const float kShootingInterval = 0.5f;

WeaponInstance::WeaponInstance() :	
	ModelInstance3D(IShader::GetShader(ShaderType::LIGHTING_SHADER), L"Assets\\Models\\Weapon.model", ModelParameters(), L"Assets\\Textures\\Weapon.dds"),
	m_Crosshair(*new ModelInstance2D(IShader::GetShader(ShaderType::TEXTURE_SHADER),
									L"Assets\\Models\\Square.model", 
									ModelParameters(), 
									L"Assets\\Textures\\Crosshair.dds")),
	m_LastShot(-kShootingInterval)
{
	m_Crosshair.SetScale(DirectX::XMFLOAT3(50.0f, 50.0f, 50.0f));
	System::GetInstance().AddModel(shared_ptr<IModelInstance>(&m_Crosshair));
}

WeaponInstance::~WeaponInstance()
{
	System::GetInstance().RemoveModel(&m_Crosshair);
}

// Returns number of zombies killed
int WeaponInstance::Fire(const vector<shared_ptr<ZombieInstanceBase>>& zombies, const DirectX::XMFLOAT3& playerPosition)
{
	using namespace DirectX;
	
	auto time = static_cast<float>(Tools::GetTime());

	if (time - m_LastShot < kShootingInterval)
	{
		return 0;
	}

	m_LastShot = time;

	XMMATRIX rotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(m_Parameters.rotation.x, m_Parameters.rotation.y, m_Parameters.rotation.z);

	XMVECTOR sourceDelta = XMVector3Transform(XMVectorSet(0.0f, 0.02f, -0.5f, 1.0f), rotationMatrix);
	XMVECTOR targetDelta = XMVector3Transform(XMVectorSet(0.0f, -2 * kWeaponPositionOffset.y, -100.0f, 1.0f), rotationMatrix);

	XMVECTOR source = XMLoadFloat3(&m_Parameters.position) + sourceDelta;
	XMVECTOR target = source + targetDelta;

	LaserProjectileInstance::Spawn(source, target);

	/*
	Does it hit the zombie?

	Assume zombie is a plane facing directly to us. It's equation: Ax + By + Cz = D
	Our projectile equation 
	S_x, S_y, S_z are laser source coordinates,
	T_x, T_y, T_z are laser target coordinates,
	D_x, D_y, D_z are laser delta coordinates:
	x = S_x + (T_x - S_x) * t = S_x + D_x * t
	y = S_y + (T_y - S_y) * t = S_y + D_y * t
	z = S_z + (T_z - S_z) * t = S_z + D_z * t

	Then, we compute at what coordinates laser intersects with the plane:

	A * (S_x + D_x * t) + B * (S_y + D_y * t) + C * (S_z + D_z * t) = D
	A * S_x + A * t * D_x + B * S_y + B * t * D_y + C* S_z + C * t * D_z = D
	t * (A * D_x + B * D_y + C * D_z) = (D - A * S_x - B * S_y - C * S_z)

	t = (D - A * S_x - B * S_y - C * S_z) / (A * D_x + B * D_y + C * D_z)
	t = (D - dot(normal, source)) / dot(normal, delta)

	*/

	XMVECTOR delta = target - source;
	auto zombiesKilled = 0;

	for (auto& zombieRef : zombies)
	{
		auto zombie = zombieRef.get();

		if (zombie->IsDead())
		{
			continue;
		}

		auto playerPosFixed = playerPosition;
		playerPosFixed.y = 0.0f;
		
		XMVECTOR zombiePosition = XMLoadFloat3(&zombie->GetPosition());
		XMVECTOR zombieNormal = XMLoadFloat3(&playerPosFixed) - zombiePosition;
		XMVECTOR zombieD = XMVector3Dot(zombiePosition, zombieNormal);		
		XMVECTOR t = (zombieD - XMVector3Dot(zombieNormal, source)) / XMVector3Dot(zombieNormal, delta);

		XMVECTOR collisionPoint = XMVectorMultiplyAdd(delta, t, source); 

		// Only register hits if zombie is in front
		if (XMVectorGetX(XMVector3Dot(collisionPoint - source, delta)) > 0.0f)
		{
			XMFLOAT3A collisionPointDelta;
			XMStoreFloat3A(&collisionPointDelta, collisionPoint - zombiePosition);
		
			float deltaX = sqrt(collisionPointDelta.x * collisionPointDelta.x + collisionPointDelta.z * collisionPointDelta.z);
		
			if (deltaX < 0.5f && collisionPointDelta.y >= 0.0f && collisionPointDelta.y < 1.5f)
			{
				if (zombie->TakeDamage(collisionPointDelta.y / 1.2f))
				{
					zombiesKilled++;
				}
			}
		}
	}

	return zombiesKilled;
}