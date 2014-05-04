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


ComPtr<ID3D11Buffer> AutoShader::CreateVertexBuffer(unsigned int vertexCount, unsigned int semanticIndex, D3D11_USAGE usage) const
{
	return m_VertexShader.CreateVertexBuffer(vertexCount, usage, semanticIndex);
}

ComPtr<ID3D11Buffer> AutoShader::CreateVertexBuffer(unsigned int vertexCount, const VertexParameters vertices[], 
													unsigned int semanticIndex, D3D11_USAGE usage) const
{
	return m_VertexShader.CreateVertexBufferAndUploadData(vertexCount, vertices, usage, semanticIndex);
}

void AutoShader::UploadVertexData(ID3D11Buffer* vertexBuffer, unsigned int vertexCount, const VertexParameters vertices[], unsigned int semanticIndex) const
{
	m_VertexShader.UploadVertexData(vertexBuffer, vertexCount, vertices, semanticIndex);
}

void AutoShader::SetRenderParameters(const RenderParameters& renderParameters)
{
	m_VertexShader.SetRenderParameters(renderParameters);
	m_PixelShader.SetRenderParameters(renderParameters);
}