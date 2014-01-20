#include "PrecompiledHeader.h"
#include "VertexShader.h"
#include "Tools.h"

VertexShader::VertexShader(ComPtr<ID3D11Device> device, wstring path)
{
	HRESULT result;

	auto shaderBuffer = Tools::ReadFileToVector(path);
	result = device->CreateVertexShader(&shaderBuffer[0], shaderBuffer.size(), nullptr, &m_Shader);
	Assert(result == S_OK);

	Reflect(device, shaderBuffer);
}


VertexShader::~VertexShader()
{
}

void VertexShader::ReflectVirtual(ComPtr<ID3D11Device> device, ComPtr<ID3D11ShaderReflection> shaderReflection, const D3D11_SHADER_DESC& shaderDescription)
{
	ShaderProgram::ReflectVirtual(device, shaderReflection, shaderDescription);

	ReflectInputLayout(device, shaderReflection, shaderDescription);
}

void VertexShader::ReflectInputLayout(ComPtr<ID3D11Device> device, ComPtr<ID3D11ShaderReflection> shaderReflection, const D3D11_SHADER_DESC& shaderDescription)
{
	HRESULT result;
	D3D11_SIGNATURE_PARAMETER_DESC parameterDescription;

	for (auto i = 0u; i < shaderDescription.InputParameters; i++)
	{
		result = shaderReflection->GetInputParameterDesc(i, &parameterDescription);
		Assert(result == S_OK);

		m_InputLayoutItems.emplace_back(device, parameterDescription);
	}
}
