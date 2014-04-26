#include "PrecompiledHeader.h"
#include "AnimatedModel.h"

AnimatedModel::AnimatedModel(IShader& shader, const wstring& modelPath) :
	IModel(shader, modelPath)
{
}
	
AnimatedModel::AnimatedModel(AnimatedModel&& other) :
	IModel(std::move(other))
{
}

AnimatedModel::~AnimatedModel()
{
}

void AnimatedModel::Render(const RenderParameters& renderParameters)
{
	throw exception("not implemented");
}