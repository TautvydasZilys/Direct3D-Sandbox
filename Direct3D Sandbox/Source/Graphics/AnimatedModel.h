#pragma once

#include "IModel.h"

class AnimatedModel :
	public IModel
{
	vector<ComPtr<ID3D11Buffer>> m_FromFrameVertexBuffers;
	vector<ComPtr<ID3D11Buffer>> m_ToFrameVertexBuffers;
	unsigned int m_TotalFrameCount;

	unsigned int m_StateCount;	
	unique_ptr<AnimatedModelState[]> m_StateData;

	AnimatedModel(IShader& shader, const wstring& modelPath);

	void CreateBuffers(const AnimatedModelData& modelData);
	virtual void SetRenderParametersAndApplyBuffers(RenderParameters& renderParameters);

	AnimatedModel(const AnimatedModel& other);												// Not implemented (no copying allowed)
	AnimatedModel& operator=(const AnimatedModel& other);									// Not implemented (no copying allowed)

	friend class std::_Ref_count_obj<AnimatedModel>;

public:
	AnimatedModel(AnimatedModel&& other);
	virtual ~AnimatedModel();
};