#include "PrecompiledHeader.h"
#include "Parameters.h"
#include "PixelShader.h"
#include "Tools.h"

PixelShader::PixelShader(ComPtr<ID3D11Device> device, wstring path)
{
	HRESULT result;

	auto shaderBuffer = Tools::ReadFileToVector(path);
	result = device->CreatePixelShader(&shaderBuffer[0], shaderBuffer.size(), nullptr, &m_Shader);
	Assert(result == S_OK);
	
	auto extensionIndex = path.find_last_of('.');
	auto metadataBuffer = Tools::ReadFileToVector(path.substr(0, extensionIndex + 1) + L"shadermetadata");

	Reflect(device, shaderBuffer, metadataBuffer);
}

PixelShader::~PixelShader()
{
}

void PixelShader::SetRenderParameters(ComPtr<ID3D11DeviceContext> deviceContext, const RenderParameters& renderParameters)
{
	ShaderProgram::SetRenderParameters(deviceContext, renderParameters);

	deviceContext->PSSetShader(m_Shader.Get(), nullptr, 0);
}

void PixelShader::SetConstantBuffersImpl(ComPtr<ID3D11DeviceContext> deviceContext) const
{
	static const PixelShader* shaderWhichLastSet = nullptr;

	if (shaderWhichLastSet != this)
	{
		shaderWhichLastSet = this;
		deviceContext->PSSetConstantBuffers(0, static_cast<UINT>(m_ConstantBufferPtrs.size()), m_ConstantBufferPtrs.data());
	}
}

void PixelShader::SetTexturesImpl(ComPtr<ID3D11DeviceContext> deviceContext)
{
	deviceContext->PSSetShaderResources(0, static_cast<UINT>(m_Textures.size()), m_Textures.data());
}

void PixelShader::SetSamplersImpl(ComPtr<ID3D11DeviceContext> deviceContext) const
{
	static const PixelShader* shaderWhichLastSet = nullptr;

	if (shaderWhichLastSet != this)
	{
		shaderWhichLastSet = this;
		deviceContext->PSSetSamplers(0, static_cast<UINT>(m_SamplerStates.size()), m_SamplerStates.data());
	}
}