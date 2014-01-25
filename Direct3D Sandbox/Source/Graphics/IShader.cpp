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

void IShader::LoadShaders(ComPtr<ID3D11Device> device)
{
	s_Shaders.resize(ShaderType::SHADER_COUNT);
	
	s_Shaders[COLOR_SHADER] = make_shared<AutoShader>(device, L"Shaders\\ColorVertexShader.cso", L"Shaders\\ColorPixelShader.cso");
}