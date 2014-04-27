#pragma once

#include "IModel.h"

class MutableModel :
	public IModel
{
	ComPtr<ID3D11Buffer> m_VertexBuffer;
	unsigned int m_Capacity;
	bool m_DirtyVertexBuffer;

	MutableModel(IShader& shader);

	virtual void SetRenderParametersAndApplyBuffers(RenderParameters& renderParameters);

	static unordered_map<IShader*, MutableModel> s_MutableModelCache;	
	friend class std::_Ref_count_obj<MutableModel>;

public:
	MutableModel(MutableModel&& other);
	virtual ~MutableModel();

	void UploadModelData(const ModelData& modelData);

	static MutableModel& GetMutableModel(IShader& shader);
};

