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


ComPtr<ID3D11Buffer> AutoShader::CreateVertexBuffer(unsigned int vertexCount, D3D11_USAGE usage) const
{
	return m_VertexShader.CreateVertexBuffer(vertexCount, usage);
}

ComPtr<ID3D11Buffer> AutoShader::CreateVertexBuffer(unsigned int vertexCount, const VertexParameters vertices[], D3D11_USAGE usage) const
{
	return m_VertexShader.CreateVertexBufferAndUploadData(vertexCount, vertices, usage);
}

void AutoShader::UploadVertexData(ID3D11Buffer* vertexBuffer, unsigned int vertexCount, const VertexParameters vertices[]) const
{
	m_VertexShader.UploadVertexData(vertexBuffer, vertexCount, vertices);
}

void AutoShader::SetRenderParameters(const RenderParameters& renderParameters)
{
	m_VertexShader.SetRenderParameters(renderParameters);
	m_PixelShader.SetRenderParameters(renderParameters);
}