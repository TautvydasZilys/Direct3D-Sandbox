#include "PrecompiledHeader.h"

#include "Source\Graphics\IShader.h"
#include "Source\Graphics\Texture.h"
#include "ModelInstance2D.h"
#include "System.h"
#include "WeaponInstance.h"

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