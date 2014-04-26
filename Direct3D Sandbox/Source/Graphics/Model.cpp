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
	)
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
	)
{
	CreateBuffers(modelData);
}

Model::Model(Model&& other) :
	IModel(std::move(other)),
	m_VertexBuffer(other.m_VertexBuffer)
{
	other.m_VertexBuffer = nullptr;
}

Model::~Model()
{
}

void Model::CreateBuffers(const ModelData& modelData)
{
	m_VertexCount = static_cast<unsigned int>(modelData.vertexCount);
	m_VertexBuffer = m_Shader.CreateVertexBuffer(m_VertexCount, modelData.vertices.get());

	InitializeIndexBuffer(modelData);
}

Model Model::CreateNonCachedModel(const ModelData& modelData, IShader& shader)
{
	return Model(shader, modelData);
}

void Model::SetRenderParametersAndApplyBuffers(RenderParameters& renderParameters)
{
	SetBuffersToDeviceContext(m_VertexBuffer.Get());
}
