#include "PrecompiledHeader.h"
#include "AutoShader.h"
#include "IShader.h"

vector<shared_ptr<IShader>> IShader::s_Shaders;

IShader::IShader()
{
}

IShader::~IShader()
{
}

void IShader::LoadShaders()
{
	s_Shaders.resize(ShaderType::SHADER_COUNT);
	
	s_Shaders[COLOR_SHADER] = make_shared<AutoShader>(L"Shaders\\ColorVertexShader.cso", L"Shaders\\ColorPixelShader.cso");
	s_Shaders[TEXTURE_SHADER] = make_shared<AutoShader>(L"Shaders\\TextureVertexShader.cso", L"Shaders\\TexturePixelShader.cso");
	s_Shaders[NORMAL_MAP_SHADER] = make_shared<AutoShader>(L"Shaders\\NormalMapVertexShader.cso", L"Shaders\\NormalMapPixelShader.cso");
	s_Shaders[PLAYGROUND_SHADER] = make_shared<AutoShader>(L"Shaders\\PlaygroundVertexShader.cso", L"Shaders\\PlaygroundPixelShader.cso");
	s_Shaders[INFINITE_GROUND_SHADER] = make_shared<AutoShader>(L"Shaders\\InfiniteGroundVertexShader.cso", L"Shaders\\LightingPixelShader.cso");	
}