#include "PrecompiledHeader.h"
#include "Direct3D.h"
#include "IShader.h"
#include "Model.h"
#include "Tools.h"

unordered_map<wstring, const ModelData> Model::s_ModelDataCache;
unordered_map<ModelId, Model, ModelIdHash> Model::s_ModelCache;

Model::Model(IShader& shader, const wstring& modelPath) :
#if DEBUG
	m_Key(modelPath),
#endif
	m_IndexCount(0),
	m_Shader(shader)
{
	auto cachedModel = s_ModelDataCache.find(modelPath);

	if (cachedModel == s_ModelDataCache.end())
	{
		s_ModelDataCache.insert(make_pair(modelPath, Tools::LoadModel(modelPath, false)));
		cachedModel = s_ModelDataCache.find(modelPath);
	}

	auto& modelData = cachedModel->second;
	m_VertexBuffer = shader.CreateVertexBuffer(modelData);
	
	m_IndexCount = static_cast<unsigned int>(modelData.indexCount);
	Assert(m_IndexCount > 0);
	
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

Model::Model(Model&& other) :
#if DEBUG
	m_Key(other.m_Key),
#endif
	m_VertexBuffer(other.m_VertexBuffer),
	m_IndexBuffer(other.m_IndexBuffer),
	m_IndexCount(other.m_IndexCount),
	m_Shader(other.m_Shader)
{
	other.m_VertexBuffer = nullptr;
	other.m_IndexBuffer = nullptr;
}

Model::~Model()
{
}

void Model::InitializeModel(IShader& shader, const wstring& modelPath)
{
	Assert(s_ModelCache.find(ModelId(modelPath, shader)) == s_ModelCache.end());

	s_ModelCache.insert(make_pair(ModelId(modelPath, shader), Model(shader, modelPath)));
}

Model& Model::Get(const wstring& modelPath, IShader& shader)
{
	auto model = s_ModelCache.find(ModelId(modelPath, shader));
	
	if (model == s_ModelCache.end())
	{
		InitializeModel(shader, modelPath);
	}

	model = s_ModelCache.find(ModelId(modelPath, shader));
	Assert(model != s_ModelCache.end());
	return model->second;
}

void Model::Render(const RenderParameters& renderParameters)
{
	auto const offset = 0u;
	static const Model* modelWhichLastSet = nullptr;
	auto deviceContext = GetD3D11DeviceContext();

	m_Shader.SetRenderParameters(renderParameters);
	
	if (modelWhichLastSet != this)
	{
		modelWhichLastSet = this;
		deviceContext->IASetVertexBuffers(0, 1, m_VertexBuffer.GetAddressOf(), m_Shader.GetInputLayoutSizePtr(), &offset);
		deviceContext->IASetIndexBuffer(m_IndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
		deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	}

	deviceContext->DrawIndexed(m_IndexCount, 0, 0);
}
