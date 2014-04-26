#pragma once

#include "IModel.h"

class AnimatedModel :
	public IModel
{
	vector<ComPtr<ID3D11Buffer>> m_VertexBuffers;
	unsigned int m_FrameCount;

	AnimatedModel(IShader& shader, const wstring& modelPath);

	void CreateBuffers(const AnimatedModelData& modelData);
	virtual void SetBuffersInD3DContext();

	AnimatedModel(const AnimatedModel& other);												// Not implemented (no copying allowed)
	AnimatedModel& operator=(const AnimatedModel& other);									// Not implemented (no copying allowed)

	friend class std::_Ref_count_obj<AnimatedModel>;

public:
	AnimatedModel(AnimatedModel&& other);
	virtual ~AnimatedModel();
};