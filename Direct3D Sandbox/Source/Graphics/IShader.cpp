#include "PrecompiledHeader.h"
#include "AutoShader.h"
#include "IShader.h"
#include "Tools.h"

vector<shared_ptr<IShader>> IShader::s_Shaders;

IShader::IShader()
{
}

IShader::~IShader()
{
}

void IShader::LoadShaders()
{
	s_Shaders.resize(ShaderType::SHADER_COUNT, nullptr);
	
	s_Shaders[ShaderType::COLOR_SHADER] = make_shared<AutoShader>(L"Shaders\\ColorVertexShader.cso", L"Shaders\\ColorPixelShader.cso");
	s_Shaders[ShaderType::TEXTURE_SHADER] = make_shared<AutoShader>(L"Shaders\\TextureVertexShader.cso", L"Shaders\\TexturePixelShader.cso");
	s_Shaders[ShaderType::LIGHTING_SHADER] = make_shared<AutoShader>(L"Shaders\\LightingVertexShader.cso", L"Shaders\\LightingPixelShader.cso");
	s_Shaders[ShaderType::NORMAL_MAP_SHADER] = make_shared<AutoShader>(L"Shaders\\NormalMapVertexShader.cso", L"Shaders\\NormalMapPixelShader.cso");
	s_Shaders[ShaderType::ANIMATION_NORMAL_MAP_SHADER] = make_shared<AutoShader>(L"Shaders\\AnimationNormalMapVertexShader.cso", L"Shaders\\NormalMapPixelShader.cso");
	s_Shaders[ShaderType::PLAYGROUND_SHADER] = make_shared<AutoShader>(L"Shaders\\PlaygroundVertexShader.cso", L"Shaders\\PlaygroundPixelShader.cso");
	s_Shaders[ShaderType::INFINITE_GROUND_SHADER] = make_shared<AutoShader>(L"Shaders\\InfiniteGroundVertexShader.cso", L"Shaders\\LightingPixelShader.cso");	

	Assert(s_Shaders.size() == ShaderType::SHADER_COUNT);
}