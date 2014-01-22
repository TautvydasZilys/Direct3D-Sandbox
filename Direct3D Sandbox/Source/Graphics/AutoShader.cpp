#include "PrecompiledHeader.h"
#include "AutoShader.h"
#include "Tools.h"

AutoShader::AutoShader(ComPtr<ID3D11Device> device, wstring vertexShaderPath, wstring pixelShaderPath) :
	m_VertexShader(device, vertexShaderPath), m_PixelShader(device, pixelShaderPath)
{
}

AutoShader::~AutoShader()
{
}

ComPtr<ID3D11Buffer> AutoShader::CreateVertexBuffer(ComPtr<ID3D11Device> device, const vector<VertexParameters>& vertices) const
{
	return m_VertexShader.CreateVertexBuffer(device, vertices);
}

void AutoShader::SetRenderParameters(ComPtr<ID3D11DeviceContext> deviceContext, const RenderParameters& renderParameters)
{
	m_VertexShader.SetRenderParameters(deviceContext, renderParameters);
	m_PixelShader.SetRenderParameters(deviceContext, renderParameters);
}