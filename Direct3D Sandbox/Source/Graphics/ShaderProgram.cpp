#include "PrecompiledHeader.h"
#include "Parameters.h"
#include "SamplerState.h"
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
	ReflectOtherResources(metadataBuffer);
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

void ShaderProgram::ReflectOtherResources(const vector<uint8_t>& metadataBuffer)
{
	using namespace Tools::BufferReader;

	auto byteOffset = 8u;
	byteOffset = ReadUInt(metadataBuffer, byteOffset);
	auto numberOfResources = ReadUInt(metadataBuffer, byteOffset);

	for (auto i = 0u; i < numberOfResources; i++)
	{
		auto resourceType = ReadUInt(metadataBuffer, byteOffset);
		auto resourceName = ReadString(metadataBuffer, byteOffset);

		switch (resourceType)
		{
		case D3D_SIT_TEXTURE:
			AddTextureOffset(resourceName);
			break;

		case D3D_SIT_SAMPLER:
			AddSamplerState(resourceName);
			break;
		}
	}
}

void ShaderProgram::AddTextureOffset(const string& name)
{
	auto offset = RenderParameters::GetFieldByteOffset(name);
	Assert(offset != 0xFFFFFFFF);

	m_TextureOffsets.push_back(offset);
}

void ShaderProgram::AddSamplerState(const string& name)
{
	auto samplerState = SamplerState::Get(name);
	m_SamplerStates.push_back(samplerState.Get());
}

void ShaderProgram::SetRenderParameters(ComPtr<ID3D11DeviceContext> deviceContext, const RenderParameters& renderParameters)
{
	for (auto& buffer : m_ConstantBuffers)
	{
		buffer.SetRenderParameters(deviceContext, renderParameters);
	}
}