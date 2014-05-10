#include "PrecompiledHeader.h"

#include "Source\Graphics\IShader.h"
#include "WeaponInstance.h"


WeaponInstance::WeaponInstance() :	
	ModelInstance(IShader::GetShader(ShaderType::TEXTURE_SHADER), L"Assets\\Models\\Weapon.model", ModelParameters(), L"Assets\\Textures\\Weapon.dds")
{
}

WeaponInstance::~WeaponInstance()
{
}
