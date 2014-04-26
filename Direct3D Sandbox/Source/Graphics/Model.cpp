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
	m_VertexCount(0)
{
	auto& modelData = GetModelData(modelPath);
	Assert(modelData.modelType == ModelType::Still);

	CreateBuffers(modelData);
}

Model::Model(IShader& shader, const ModelData& modelData) :
	IModel(shader
#if DEBUG
	, L""
#endif
	),
	m_VertexCount(0)
{
	CreateBuffers(modelData);
}

Model::Model(Model&& other) :
	IModel(std::move(other)),
	m_VertexBuffer(other.m_VertexBuffer),
	m_VertexCount(other.m_VertexCount)
{
	other.m_VertexBuffer = nullptr;
}

Model::~Model()
{
}

void Model::CreateBuffers(const ModelData& modelData)
{
	m_VertexBuffer = m_Shader.CreateVertexBuffer(modelData);
	m_VertexCount = static_cast<unsigned int>(modelData.vertexCount);

	InitializeIndexBuffer(modelData);
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
