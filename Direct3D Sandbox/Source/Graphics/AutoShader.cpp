#include "PrecompiledHeader.h"
#include "AutoShader.h"
#include "Tools.h"

AutoShader::AutoShader(wstring vertexShaderPath, wstring pixelShaderPath) :
	m_VertexShader(vertexShaderPath), m_PixelShader(pixelShaderPath)
{
}

AutoShader::~AutoShader()
{
}

ComPtr<ID3D11Buffer> AutoShader::CreateVertexBuffer(const ModelData& model) const
{
	return m_VertexShader.CreateVertexBuffer(model);
}

void AutoShader::SetRenderParameters(const RenderParameters& renderParameters)
{
	m_VertexShader.SetRenderParameters(renderParameters);
	m_PixelShader.SetRenderParameters(renderParameters);
}