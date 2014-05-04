#include "PrecompiledHeader.h"
#include "AnimatedModel.h"
#include "Direct3D.h"
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
		m_FromFrameVertexBuffers.push_back(m_Shader.CreateVertexBuffer(m_VertexCount, modelData.vertices.get() + m_VertexCount * i, 0));
		m_ToFrameVertexBuffers.push_back(m_Shader.CreateVertexBuffer(m_VertexCount, modelData.vertices.get() + m_VertexCount * i, 1));
	}

	m_StateCount = static_cast<unsigned int>(modelData.stateCount);
	m_StateData = unique_ptr<AnimatedModelState[]>(new AnimatedModelState[modelData.stateCount]);
	memcpy(m_StateData.get(), modelData.stateData.get(), m_StateCount * sizeof(AnimatedModelState));

	InitializeIndexBuffer(modelData);
}

void AnimatedModel::SetRenderParametersAndApplyBuffers(RenderParameters& renderParameters)
{
	static int s_LastFrameSet = -1;
	int currentFrame;

	Assert(renderParameters.animationProgress >= 0.0f && renderParameters.animationProgress <= 1.0f);
	
	bool shouldSetVertexBuffer = false;
	ID3D11Buffer* buffers[2];

//	if (!renderParameters.isTransitioningAnimationStates)
//	{	
		auto currentFrameFloat = renderParameters.animationProgress * m_StateData[renderParameters.currentAnimationState].frameCount;
		currentFrame = static_cast<int>(currentFrameFloat);

		renderParameters.currentFrameProgress = currentFrameFloat - currentFrame;
	
		currentFrame += static_cast<int>(m_StateData[renderParameters.currentAnimationState].frameOffset);


		if (!DidThisLastSet() || s_LastFrameSet != currentFrame)
		{
			buffers[0] = m_FromFrameVertexBuffers[currentFrame].Get();
			buffers[1] = m_ToFrameVertexBuffers[(currentFrame + 1) % m_ToFrameVertexBuffers.size()].Get();
			
			shouldSetVertexBuffer = true;
		}
		
		s_LastFrameSet = currentFrame;
//	}

	if (shouldSetVertexBuffer)
	{
		auto deviceContext = GetD3D11DeviceContext();
		deviceContext->IASetVertexBuffers(0, 2, buffers, m_Shader.GetInputLayoutStrides(), m_Shader.GetInputLayoutOffsets());
	}
	
	SetIndexBufferToDeviceContext();
}