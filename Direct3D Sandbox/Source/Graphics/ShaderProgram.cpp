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

void ShaderProgram::Reflect(ComPtr<ID3D11Device> device, const vector<uint8_t>& shaderBuffer, const vector<uint8_t>& metadataBuffer)
{
	ReflectConstantBuffers(device, metadataBuffer);
}

void ShaderProgram::ReflectConstantBuffers(ComPtr<ID3D11Device> device, const vector<uint8_t>& metadataBuffer)
{
	using namespace Tools::BufferReader;

	auto byteOffset = 0u;
	byteOffset = ReadUInt(metadataBuffer, byteOffset);
	auto numberOfConstantBuffers = ReadUInt(metadataBuffer, byteOffset);

	m_ConstantBuffers.reserve(numberOfConstantBuffers);
	m_ConstantBufferPtrs.reserve(numberOfConstantBuffers);

	for (auto i = 0u; i < numberOfConstantBuffers; i++)
	{
		m_ConstantBuffers.emplace_back(device, metadataBuffer, byteOffset);
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