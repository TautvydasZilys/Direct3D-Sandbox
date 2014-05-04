#include "PrecompiledHeader.h"
#include "Direct3D.h"
#include "IShader.h"
#include "MutableModel.h"

unordered_map<IShader*, MutableModel> MutableModel::s_MutableModelCache;


MutableModel::MutableModel(IShader& shader) :
	IModel(shader
#if DEBUG
	, L"MutableModel"
#endif
	),
	m_Capacity(0),
	m_DirtyVertexBuffer(false)
{
}

MutableModel::MutableModel(MutableModel&& other) :
	IModel(std::move(other)),
	m_VertexBuffer(other.m_VertexBuffer),
	m_Capacity(other.m_Capacity),
	m_DirtyVertexBuffer(true)
{
	other.m_VertexBuffer = nullptr;
	other.m_Capacity = 0;
}

MutableModel::~MutableModel()
{
}

void MutableModel::SetRenderParametersAndApplyBuffers(RenderParameters& renderParameters)
{
	if (m_DirtyVertexBuffer || !DidThisLastSet())
	{
		const UINT offset = 0u;
		auto deviceContext = GetD3D11DeviceContext();

		deviceContext->IASetVertexBuffers(0, 1, m_VertexBuffer.GetAddressOf(), m_Shader.GetInputLayoutStrides(), &offset);
		m_DirtyVertexBuffer = false;
	}

	if (!DidThisLastSet())
	{
		SetIndexBufferToDeviceContext();
	}
}

void MutableModel::UploadModelData(const ModelData& modelData)
{
	if (m_Capacity < modelData.vertexCount)
	{
		while (m_Capacity < modelData.vertexCount)
		{
			m_Capacity = m_Capacity * 2 + 1;
		}

		m_VertexBuffer = m_Shader.CreateVertexBuffer(m_Capacity, 0, D3D11_USAGE::D3D11_USAGE_DYNAMIC);
		Assert(m_VertexBuffer != nullptr);

		m_DirtyVertexBuffer = true;
	}

	m_Shader.UploadVertexData(m_VertexBuffer.Get(), static_cast<unsigned int>(modelData.vertexCount), modelData.vertices.get(), 0);
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