#include "PrecompiledHeader.h"
#include "PixelShader.h"
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

void PixelShader::SetRenderParameters(ComPtr<ID3D11DeviceContext> deviceContext, const RenderParameters& renderParameters)
{
	ShaderProgram::SetRenderParameters(deviceContext, renderParameters);

	deviceContext->PSSetShader(m_Shader.Get(), nullptr, 0);
}