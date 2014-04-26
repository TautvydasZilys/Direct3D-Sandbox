#include "PrecompiledHeader.h"

#include "IModel.h"
#include "AnimatedModel.h"
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
	m_Shader(other.m_Shader)
#if DEBUG
	, m_Key(std::move(other.m_Key))
#endif
{
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