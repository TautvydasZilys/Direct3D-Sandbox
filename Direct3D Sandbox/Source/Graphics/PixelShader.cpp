#include "PrecompiledHeader.h"
#include "Direct3D.h"
#include "Parameters.h"
#include "PixelShader.h"
#include "Tools.h"

PixelShader::PixelShader(wstring path)
{
	HRESULT result;

	auto shaderBuffer = Tools::ReadFileToVector(path);
	result = GetD3D11Device()->CreatePixelShader(&shaderBuffer[0], shaderBuffer.size(), nullptr, &m_Shader);
	Assert(result == S_OK);
	
	auto extensionIndex = path.find_last_of('.');
	auto metadataBuffer = Tools::ReadFileToVector(path.substr(0, extensionIndex + 1) + L"shadermetadata");

	Reflect(shaderBuffer, metadataBuffer);
}

PixelShader::~PixelShader()
{
}

void PixelShader::SetRenderParameters(const RenderParameters& renderParameters)
{
	ShaderProgram::SetRenderParameters(renderParameters);
	
	static const PixelShader* shaderWhichLastSet = nullptr;
	if (shaderWhichLastSet != this)
	{
		shaderWhichLastSet = this;
		GetD3D11DeviceContext()->PSSetShader(m_Shader.Get(), nullptr, 0);
	}
}

void PixelShader::SetConstantBuffersImpl() const
{
	static const PixelShader* shaderWhichLastSet = nullptr;

	if (shaderWhichLastSet != this)
	{
		shaderWhichLastSet = this;
		GetD3D11DeviceContext()->PSSetConstantBuffers(0, static_cast<UINT>(m_ConstantBufferPtrs.size()), m_ConstantBufferPtrs.data());
	}
}

void PixelShader::SetTexturesImpl()
{
	GetD3D11DeviceContext()->PSSetShaderResources(0, static_cast<UINT>(m_Textures.size()), m_Textures.data());
}

void PixelShader::SetSamplersImpl() const
{
	static const PixelShader* shaderWhichLastSet = nullptr;

	if (shaderWhichLastSet != this)
	{
		shaderWhichLastSet = this;
		GetD3D11DeviceContext()->PSSetSamplers(0, static_cast<UINT>(m_SamplerStates.size()), m_SamplerStates.data());
	}
}