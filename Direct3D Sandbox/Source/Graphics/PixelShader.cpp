#include "PrecompiledHeader.h"
#include "Parameters.h"
#include "PixelShader.h"
#include "SamplerState.h"
#include "Tools.h"

PixelShader::PixelShader(ComPtr<ID3D11Device> device, wstring path)
{
	HRESULT result;

	auto shaderBuffer = Tools::ReadFileToVector(path);
	result = device->CreatePixelShader(&shaderBuffer[0], shaderBuffer.size(), nullptr, &m_Shader);
	Assert(result == S_OK);

	Reflect(device, shaderBuffer);
}

PixelShader::~PixelShader()
{
}

void PixelShader::ReflectVirtual(ComPtr<ID3D11Device> device, const vector<uint8_t>& shaderBuffer, ComPtr<ID3D11ShaderReflection> shaderReflection, 
									const D3D11_SHADER_DESC& shaderDescription)
{
	ShaderProgram::ReflectVirtual(device, shaderBuffer, shaderReflection, shaderDescription);

	HRESULT result;
	D3D11_SHADER_INPUT_BIND_DESC desc;

	for (auto i = 0u; i < shaderDescription.BoundResources; i++)
	{
		result = shaderReflection->GetResourceBindingDesc(i, &desc);
		Assert(result == S_OK);

		switch (desc.Type)
		{
		case D3D_SIT_TEXTURE:
			AddTextureOffset(desc.Name);
			break;

		case D3D_SIT_SAMPLER:
			AddSamplerState(desc.Name);
			break;
		}
	}
}

void PixelShader::AddTextureOffset(const string& name)
{
	auto offset = RenderParameters::GetFieldByteOffset(name);
	Assert(offset != 0xFFFFFFFF);

	m_TextureOffsets.push_back(offset);
}

void PixelShader::AddSamplerState(const string& name)
{
	auto samplerState = SamplerState::Get(name);
	m_SamplerStates.push_back(samplerState.Get());
}

void PixelShader::SetRenderParameters(ComPtr<ID3D11DeviceContext> deviceContext, const RenderParameters& renderParameters)
{
	ShaderProgram::SetRenderParameters(deviceContext, renderParameters);

	SetConstantBuffers(deviceContext);
	SetTextures(deviceContext, renderParameters);
	SetSamplers(deviceContext);

	deviceContext->PSSetShader(m_Shader.Get(), nullptr, 0);
}

void PixelShader::SetConstantBuffers(ComPtr<ID3D11DeviceContext> deviceContext) const
{
	if (m_ConstantBufferPtrs.size() > 0)
	{
		deviceContext->PSSetConstantBuffers(0, static_cast<UINT>(m_ConstantBufferPtrs.size()), m_ConstantBufferPtrs.data());
	}
}

void PixelShader::SetTextures(ComPtr<ID3D11DeviceContext> deviceContext, const RenderParameters& renderParameters) const
{
	const auto textureCount = m_TextureOffsets.size();

	if (textureCount > 0)
	{
		unique_ptr<ID3D11ShaderResourceView*[]> textures(new ID3D11ShaderResourceView*[textureCount]);

		for (auto i = 0u; i < textureCount; i++)
		{
			memcpy(&textures[i], reinterpret_cast<const uint8_t*>(&renderParameters) + m_TextureOffsets[i], sizeof(ID3D11ShaderResourceView));
		}

		deviceContext->PSSetShaderResources(0, static_cast<UINT>(textureCount), textures.get());
	}
}

void PixelShader::SetSamplers(ComPtr<ID3D11DeviceContext> deviceContext) const
{
	if (m_SamplerStates.size() > 0)
	{
		deviceContext->PSSetSamplers(0, static_cast<UINT>(m_SamplerStates.size()), m_SamplerStates.data());
	}
}