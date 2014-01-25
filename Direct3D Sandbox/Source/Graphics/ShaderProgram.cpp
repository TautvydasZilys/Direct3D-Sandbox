#include "PrecompiledHeader.h"
#include "Parameters.h"
#include "ShaderProgram.h"
#include "Tools.h"

ShaderProgram::ShaderProgram()
{	
}

ShaderProgram::~ShaderProgram()
{
}

void ShaderProgram::Reflect(ComPtr<ID3D11Device> device, const vector<uint8_t>& shaderBuffer)
{
	HRESULT result;
	ComPtr<ID3D11ShaderReflection> shaderReflection;
	D3D11_SHADER_DESC shaderDescription;

	result = D3DReflect(shaderBuffer.data(), shaderBuffer.size(), IID_ID3D11ShaderReflection, &shaderReflection);
	Assert(result == S_OK);
		
	result = shaderReflection->GetDesc(&shaderDescription);
	Assert(result == S_OK);

	ReflectVirtual(device, shaderBuffer, shaderReflection, shaderDescription);
}

void ShaderProgram::ReflectVirtual(ComPtr<ID3D11Device> device, const vector<uint8_t>& shaderBuffer, ComPtr<ID3D11ShaderReflection> shaderReflection,
									const D3D11_SHADER_DESC& shaderDescription)
{
	ReflectConstantBuffers(device, shaderReflection, shaderDescription);
}

void ShaderProgram::ReflectConstantBuffers(ComPtr<ID3D11Device> device, ComPtr<ID3D11ShaderReflection> shaderReflection, const D3D11_SHADER_DESC& shaderDescription)
{
	m_ConstantBuffers.reserve(shaderDescription.ConstantBuffers);
	m_ConstantBufferPtrs.reserve(shaderDescription.ConstantBuffers);

	for (auto i = 0u; i < shaderDescription.ConstantBuffers; i++)
	{
		auto buffer = shaderReflection->GetConstantBufferByIndex(i);
		m_ConstantBuffers.emplace_back(device, buffer);
		m_ConstantBufferPtrs.push_back(m_ConstantBuffers[i].GetPtr());
	}
}

void ShaderProgram::SetRenderParameters(ComPtr<ID3D11DeviceContext> deviceContext, const RenderParameters& renderParameters)
{
	for (auto& buffer : m_ConstantBuffers)
	{
		buffer.SetRenderParameters(deviceContext, renderParameters);
	}
}