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
	int currentFrame, nextFrame;

	Assert(renderParameters.currentStateAnimationProgress >= 0.0f && renderParameters.currentStateAnimationProgress <= 1.0f);
	
	bool shouldSetVertexBuffer = false;
	
	auto currentFrameFloat = renderParameters.currentStateAnimationProgress * m_StateData[renderParameters.currentAnimationState].frameCount;
	currentFrame = static_cast<int>(currentFrameFloat);

	if (!renderParameters.isTransitioningAnimationStates)
	{	
		nextFrame = (currentFrame + 1) % m_StateData[renderParameters.currentAnimationState].frameCount;
	
		renderParameters.currentFrameProgress = currentFrameFloat - currentFrame;
		currentFrame += static_cast<int>(m_StateData[renderParameters.currentAnimationState].frameOffset);
		nextFrame += static_cast<int>(m_StateData[renderParameters.currentAnimationState].frameOffset);

		shouldSetVertexBuffer = !DidThisLastSet() || s_LastFrameSet != currentFrame;		
		s_LastFrameSet = currentFrame;
	}
	else
	{
		auto targetFrameFloat = renderParameters.targetStateAnimationProgress * m_StateData[renderParameters.targetAnimationState].frameCount;
		nextFrame = static_cast<int>(targetFrameFloat);
		
		renderParameters.currentFrameProgress = renderParameters.transitionProgress;
		currentFrame += static_cast<int>(m_StateData[renderParameters.currentAnimationState].frameOffset);
		nextFrame += static_cast<int>(m_StateData[renderParameters.targetAnimationState].frameOffset);		

		shouldSetVertexBuffer = true;
		s_LastFrameSet = -1;
	}

	if (shouldSetVertexBuffer)
	{
		const UINT offsets[] = { 0u, 0u };
		auto deviceContext = GetD3D11DeviceContext();

		ID3D11Buffer* buffers[] = 
		{
			m_FromFrameVertexBuffers[currentFrame].Get(),
			m_ToFrameVertexBuffers[nextFrame].Get()
		};

		deviceContext->IASetVertexBuffers(0, 2, buffers, m_Shader.GetInputLayoutStrides(), offsets);
	}
	
	SetIndexBufferToDeviceContext();
}