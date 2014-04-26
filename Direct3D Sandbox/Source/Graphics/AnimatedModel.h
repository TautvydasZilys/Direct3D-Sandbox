#pragma once

#include "IModel.h"

class AnimatedModel :
	public IModel
{	
	AnimatedModel(IShader& shader, const wstring& modelPath);
	
	AnimatedModel(const AnimatedModel& other);												// Not implemented (no copying allowed)
	AnimatedModel& operator=(const AnimatedModel& other);									// Not implemented (no copying allowed)
	
	friend class std::_Ref_count_obj<AnimatedModel>;

public:
	AnimatedModel(AnimatedModel&& other);
	virtual ~AnimatedModel();

	virtual void Render(const RenderParameters& renderParameters);
};