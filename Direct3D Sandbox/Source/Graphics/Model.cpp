#include "PrecompiledHeader.h"
#include "IShader.h"
#include "Model.h"
#include "Tools.h"

unordered_map<string, const ModelData> Model::s_ModelDataCache;
unordered_map<ModelId, Model, ModelIdHash> Model::s_ModelCache;

Model::Model(ComPtr<ID3D11Device> device, IShader& shader, const string& modelPath) :
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
	m_VertexBuffer = shader.CreateVertexBuffer(device, modelData);

	// TO DO: Create index buffer here

	Assert(m_IndexCount > 0);
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

void Model::InitializeModel(ComPtr<ID3D11Device> device, IShader& shader, const string& modelPath)
{
	Assert(s_ModelCache.find(ModelId(modelPath, shader)) == s_ModelCache.end());

	s_ModelCache.insert(make_pair(ModelId(modelPath, shader), Model(device, shader, modelPath)));
}

Model& Model::Get(const string& modelPath, IShader& shader)
{
	auto model = s_ModelCache.find(ModelId(modelPath, shader));
	
	Assert(model != s_ModelCache.end());
	return model->second;
}

void Model::Render(ComPtr<ID3D11DeviceContext> deviceContext, const RenderParameters& renderParameters)
{
	auto const offset = 0u;

	m_Shader.SetRenderParameters(deviceContext, renderParameters);

	deviceContext->IASetVertexBuffers(0, 1, m_VertexBuffer.GetAddressOf(), m_Shader.GetInputLayoutSizePtr(), &offset);
	deviceContext->IASetIndexBuffer(m_IndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	deviceContext->DrawIndexed(m_IndexCount, 0, 0);
}
