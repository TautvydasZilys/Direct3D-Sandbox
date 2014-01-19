#include "PrecompiledHeader.h"
#include "ShaderProgram.h"
#include "Tools.h"

ShaderProgram::ShaderProgram()
{	
}

ShaderProgram::~ShaderProgram()
{
}

void ShaderProgram::Reflect(const vector<uint8_t>& shaderBuffer)
{
	HRESULT result;
	ComPtr<ID3D11ShaderReflection> shaderReflection;
	D3D11_SHADER_DESC shaderDescription;

	result = D3DReflect(shaderBuffer.data(), shaderBuffer.size(), IID_ID3D11ShaderReflection, &shaderReflection);
	Assert(result == S_OK);
		
	result = shaderReflection->GetDesc(&shaderDescription);
	Assert(result == S_OK);

	ReflectConstantBuffers(shaderReflection, shaderDescription);
}

void ShaderProgram::ReflectConstantBuffers(ComPtr<ID3D11ShaderReflection> shaderReflection, const D3D11_SHADER_DESC& shaderDescription)
{
	HRESULT result;
	ComPtr<ID3D11ShaderReflectionConstantBuffer> buffer;

	for (auto i = 0u; i < shaderDescription.ConstantBuffers; i++)
	{
		buffer = shaderReflection->GetConstantBufferByIndex(i);
		m_ConstantBuffers.emplace_back(buffer);
	}
}