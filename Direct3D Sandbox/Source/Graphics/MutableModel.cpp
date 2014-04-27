#include "PrecompiledHeader.h"
#include "IShader.h"
#include "MutableModel.h"

unordered_map<IShader*, MutableModel> MutableModel::s_MutableModelCache;


MutableModel::MutableModel(IShader& shader) :
	IModel(shader
#if DEBUG
	, L"MutableModel"
#endif
	),
	m_Capacity(0)
{
}

MutableModel::MutableModel(MutableModel&& other) :
	IModel(std::move(other)),
	m_VertexBuffer(other.m_VertexBuffer),
	m_Capacity(other.m_Capacity)
{
	other.m_VertexBuffer = nullptr;
	other.m_Capacity = 0;
}

MutableModel::~MutableModel()
{
}

void MutableModel::SetRenderParametersAndApplyBuffers(RenderParameters& renderParameters)
{
	SetBuffersToDeviceContext(m_VertexBuffer.Get());
}

void MutableModel::UploadModelData(const ModelData& modelData)
{
	if (m_Capacity < modelData.vertexCount)
	{
		while (m_Capacity < modelData.vertexCount)
		{
			m_Capacity = m_Capacity * 2 + 1;
		}

		m_VertexBuffer = m_Shader.CreateVertexBuffer(m_Capacity, D3D11_USAGE::D3D11_USAGE_DYNAMIC);
	}
	
	m_Shader.UploadVertexData(m_VertexBuffer.Get(), static_cast<unsigned int>(modelData.vertexCount), modelData.vertices.get());
	m_VertexCount = static_cast<unsigned int>(modelData.vertexCount);
}

MutableModel& MutableModel::GetMutableModel(IShader& shader)
{
	auto model = s_MutableModelCache.find(&shader);

	if (model == s_MutableModelCache.end())
	{
		s_MutableModelCache.emplace(&shader, MutableModel(shader));
		model = s_MutableModelCache.find(&shader);
	}

	Assert(model != s_MutableModelCache.end());
	return model->second;
}