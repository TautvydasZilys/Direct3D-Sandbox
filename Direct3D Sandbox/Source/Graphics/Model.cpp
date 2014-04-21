#include "PrecompiledHeader.h"
#include "Direct3D.h"
#include "IShader.h"
#include "Model.h"
#include "Tools.h"


Model::Model(IShader& shader, const wstring& modelPath) :
	IModel(shader
#if DEBUG
	, modelPath
#endif
	),
	m_VertexCount(0),
	m_IndexCount(0)
{
	auto& modelData = GetModelData(modelPath);
	CreateBuffers(modelData);
}

Model::Model(IShader& shader, const ModelData& modelData) :
	IModel(shader
#if DEBUG
	, L""
#endif
	),
	m_VertexCount(0),
	m_IndexCount(0)
{
	CreateBuffers(modelData);
}

Model::Model(Model&& other) :
	IModel(std::move(other)),
	m_VertexBuffer(other.m_VertexBuffer),
	m_IndexBuffer(other.m_IndexBuffer),
	m_VertexCount(other.m_VertexCount),
	m_IndexCount(other.m_IndexCount)
{
	other.m_VertexBuffer = nullptr;
	other.m_IndexBuffer = nullptr;
}

Model::~Model()
{
}

void Model::CreateBuffers(const ModelData& modelData)
{
	m_VertexBuffer = m_Shader.CreateVertexBuffer(modelData);
	m_VertexCount = static_cast<unsigned int>(modelData.vertexCount);
	m_IndexCount = static_cast<unsigned int>(modelData.indexCount);
	
	if (m_IndexCount > 0)
	{
		D3D11_BUFFER_DESC indexBufferDescription;
		D3D11_SUBRESOURCE_DATA indexData;

		indexBufferDescription.Usage = D3D11_USAGE_IMMUTABLE;
		indexBufferDescription.ByteWidth = sizeof(unsigned int) * m_IndexCount;
		indexBufferDescription.BindFlags = D3D11_BIND_INDEX_BUFFER;
		indexBufferDescription.CPUAccessFlags = 0;
		indexBufferDescription.MiscFlags = 0;
		indexBufferDescription.StructureByteStride = 0;

		indexData.pSysMem = modelData.indices.get();
		indexData.SysMemPitch = 0;
		indexData.SysMemSlicePitch = 0;
	
		auto result = GetD3D11Device()->CreateBuffer(&indexBufferDescription, &indexData, &m_IndexBuffer);
		Assert(result == S_OK);
	}
}

Model Model::CreateNonCachedModel(const ModelData& modelData, IShader& shader)
{
	return Model(shader, modelData);
}

void Model::Render(const RenderParameters& renderParameters)
{
	auto const offset = 0u;
	auto deviceContext = GetD3D11DeviceContext();

	m_Shader.SetRenderParameters(renderParameters);
	
	if (s_ModelWhichLastSetParameters != this)
	{
		s_ModelWhichLastSetParameters = this;
		deviceContext->IASetVertexBuffers(0, 1, m_VertexBuffer.GetAddressOf(), m_Shader.GetInputLayoutSizePtr(), &offset);
		deviceContext->IASetIndexBuffer(m_IndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
		deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	}

	if (m_IndexCount > 0)
	{
		deviceContext->DrawIndexed(m_IndexCount, 0, 0);
	}
	else
	{
		deviceContext->Draw(m_VertexCount, 0);
	}
}
