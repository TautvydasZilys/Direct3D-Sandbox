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

void AutoShader::Render(ComPtr<ID3D11DeviceContext> deviceContext, const RenderParameters& renderParameters)
{

}