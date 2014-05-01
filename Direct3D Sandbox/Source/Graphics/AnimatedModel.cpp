#include "PrecompiledHeader.h"
#include "AnimatedModel.h"
#include "IShader.h"

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
	m_TotalFrameCount = static_cast<unsigned int>(modelData.totalFrameCount);
	m_VertexCount = static_cast<unsigned int>(modelData.vertexCount);
	
	for (auto i = 0u; i < m_TotalFrameCount; i++)
	{
		m_VertexBuffers.push_back(m_Shader.CreateVertexBuffer(m_VertexCount, modelData.vertices.get() + m_VertexCount * i));
	}

	m_StateCount = static_cast<unsigned int>(modelData.stateCount);
	m_StateData = unique_ptr<AnimatedModelState[]>(new AnimatedModelState[modelData.stateCount]);
	memcpy(m_StateData.get(), modelData.stateData.get(), m_StateCount * sizeof(AnimatedModelState));

	InitializeIndexBuffer(modelData);
}

void AnimatedModel::SetRenderParametersAndApplyBuffers(RenderParameters& renderParameters)
{
	static int s_LastFrameSet = -1;

	Assert(renderParameters.animationProgress >= 0.0f && renderParameters.animationProgress <= 1.0f);

	auto currentFrameFloat = renderParameters.animationProgress * m_StateData[renderParameters.currentAnimationState].frameCount;
	auto currentFrame = static_cast<int>(currentFrameFloat);

	renderParameters.currentFrameProgress = currentFrameFloat - currentFrame;
	
	currentFrame += static_cast<int>(m_StateData[renderParameters.currentAnimationState].frameOffset);
	SetBuffersToDeviceContext(m_VertexBuffers[currentFrame].Get(), s_LastFrameSet != currentFrame);
	s_LastFrameSet = currentFrame;
}