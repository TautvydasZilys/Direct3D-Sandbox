#include "PrecompiledHeader.h"

#include "IModel.h"
#include "AnimatedModel.h"
#include "Direct3D.h"
#include "Model.h"

unordered_map<wstring, unique_ptr<const ModelData>> IModel::s_ModelDataCache;
unordered_map<ModelId, shared_ptr<IModel>, ModelIdHash> IModel::s_ModelCache;
const IModel* IModel::s_ModelWhichLastSetParameters;

IModel::IModel(IShader& shader
#if DEBUG
		, const wstring& modelPath
#endif
		) :
	m_Shader(shader)
#if DEBUG
	, m_Key(modelPath)
#endif
{
}

IModel::IModel(IModel&& other) :
	m_Shader(other.m_Shader),
	m_IndexBuffer(other.m_IndexBuffer),
	m_IndexCount(other.m_IndexCount)
#if DEBUG
	, m_Key(std::move(other.m_Key))
#endif
{
	other.m_IndexBuffer = nullptr;
}

IModel::~IModel()
{
}

void IModel::InitializeModel(IShader& shader, const wstring& modelPath)
{
	shared_ptr<IModel> model;

	Assert(s_ModelCache.find(ModelId(modelPath, shader)) == s_ModelCache.end());

	auto modelType = GetModelData(modelPath).modelType;
	Assert(modelType < ModelType::ModelTypeCount);

	switch (modelType)
	{
	case ModelType::Still:
		model = make_shared<Model>(shader, modelPath);
		break;

	case ModelType::Animated:
		model = make_shared<AnimatedModel>(shader, modelPath);
		break;
	}

	s_ModelCache.insert(make_pair(ModelId(modelPath, shader), make_shared<Model>(shader, modelPath)));
}

IModel& IModel::Get(const wstring& modelPath, IShader& shader)
{
	auto model = s_ModelCache.find(ModelId(modelPath, shader));
	
	if (model == s_ModelCache.end())
	{
		InitializeModel(shader, modelPath);
		model = s_ModelCache.find(ModelId(modelPath, shader));
	}

	Assert(model != s_ModelCache.end());
	return *model->second;
}

const ModelData& IModel::GetModelData(const wstring& modelPath)
{	
	auto cachedModel = s_ModelDataCache.find(modelPath);

	if (cachedModel == s_ModelDataCache.end())
	{
		s_ModelDataCache.insert(make_pair(modelPath, Tools::LoadModel(modelPath)));
		cachedModel = s_ModelDataCache.find(modelPath);
	}

	return *cachedModel->second;
}

void IModel::InitializeIndexBuffer(const ModelData& modelData)
{
	m_IndexCount = static_cast<unsigned int>(modelData.indexCount);
	
	if (m_IndexCount > 0)
	{
		m_IndexBuffer = CreateIndexBuffer(m_IndexCount, modelData.indices.get());
	}
}

ComPtr<ID3D11Buffer> IModel::CreateIndexBuffer(unsigned int indexCount, unsigned int indices[])
{
	D3D11_BUFFER_DESC indexBufferDescription;
	D3D11_SUBRESOURCE_DATA indexData;
	ComPtr<ID3D11Buffer> indexBuffer;

	indexBufferDescription.Usage = D3D11_USAGE_IMMUTABLE;
	indexBufferDescription.ByteWidth = sizeof(unsigned int) * indexCount;
	indexBufferDescription.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDescription.CPUAccessFlags = 0;
	indexBufferDescription.MiscFlags = 0;
	indexBufferDescription.StructureByteStride = 0;

	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;
	
	auto result = GetD3D11Device()->CreateBuffer(&indexBufferDescription, &indexData, &indexBuffer);
	Assert(result == S_OK);

	return indexBuffer;
}
