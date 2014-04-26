#include "PrecompiledHeader.h"
#include "AnimatedModel.h"

AnimatedModel::AnimatedModel(IShader& shader, const wstring& modelPath) :
	IModel(shader
#if DEBUG
	, modelPath
#endif
	)
{
	auto& modelData = GetModelData(modelPath);
	Assert(modelData.modelType == ModelType::Animated);

	CreateBuffers(reinterpret_cast<const AnimatedModelData&>(modelData));
}

AnimatedModel::AnimatedModel(AnimatedModel&& other) :
	IModel(std::move(other))
{
}

AnimatedModel::~AnimatedModel()
{
}

void AnimatedModel::CreateBuffers(const AnimatedModelData& modelData)
{
	throw exception("not implemented");
}

void AnimatedModel::Render(const RenderParameters& renderParameters)
{
	throw exception("not implemented");
}