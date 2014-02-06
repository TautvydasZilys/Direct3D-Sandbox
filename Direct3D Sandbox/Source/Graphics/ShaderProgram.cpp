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

void ShaderProgram::Reflect(const vector<uint8_t>& shaderBuffer, const vector<uint8_t>& metadataBuffer)
{
	ReflectConstantBuffers(metadataBuffer);
	ReflectOtherResources(metadataBuffer);
}

void ShaderProgram::ReflectConstantBuffers(const vector<uint8_t>& metadataBuffer)
{
	using namespace Tools::BufferReader;

	auto byteOffset = 0u;
	byteOffset = ReadUInt(metadataBuffer, byteOffset);
	auto numberOfConstantBuffers = ReadUInt(metadataBuffer, byteOffset);

	m_ConstantBuffers.reserve(numberOfConstantBuffers);
	m_ConstantBufferPtrs.reserve(numberOfConstantBuffers);

	for (auto i = 0u; i < numberOfConstantBuffers; i++)
	{
		m_ConstantBuffers.emplace_back(metadataBuffer, byteOffset);
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

	m_Textures.resize(m_TextureOffsets.size());
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

void ShaderProgram::SetRenderParameters(const RenderParameters& renderParameters)
{
	for (auto& buffer : m_ConstantBuffers)
	{
		buffer.SetRenderParameters(renderParameters);
	}
	
	SetConstantBuffers();
	SetTextures(renderParameters);
	SetSamplers();
}

void ShaderProgram::SetConstantBuffers() const
{
	if (m_ConstantBufferPtrs.size() > 0)
	{
		SetConstantBuffersImpl();
	}
}

void ShaderProgram::SetTextures(const RenderParameters& renderParameters)
{	
	const auto textureCount = m_TextureOffsets.size();
	static ShaderProgram* shaderWhichLastSet = nullptr;

	if (textureCount > 0)
	{
		bool textureChanged = false;

		for (auto i = 0u; i < textureCount; i++)
		{
			if (memcmp(&m_Textures[i], reinterpret_cast<const uint8_t*>(&renderParameters) + m_TextureOffsets[i], sizeof(ID3D11ShaderResourceView)) != 0)
			{
				textureChanged = true;
				memcpy(&m_Textures[i], reinterpret_cast<const uint8_t*>(&renderParameters) + m_TextureOffsets[i], sizeof(ID3D11ShaderResourceView));
			}
		}

		if (shaderWhichLastSet != this || textureChanged)
		{
			shaderWhichLastSet = this;
			SetTexturesImpl();
		}
	}
}

void ShaderProgram::SetSamplers() const
{
	if (m_SamplerStates.size() > 0)
	{
		SetSamplersImpl();
	}
}