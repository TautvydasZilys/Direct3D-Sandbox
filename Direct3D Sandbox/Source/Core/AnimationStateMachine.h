#pragma once

#include "Parameters.h"

#define ANIMATION_STATE_MACHINE_TEMPLATE_PARAMETERS \
	typename States, int StateCount, const float kAnimationLengths[StateCount], const bool kDoesAnimationLoop[StateCount], const float& kAnimationTransitionLength

template <ANIMATION_STATE_MACHINE_TEMPLATE_PARAMETERS>
class AnimationStateMachine
{
	States m_CurrentState;
	States m_NextState;
	float m_AnimationProgress[StateCount];
	bool m_IsTransitioningAnimationStates;
	float m_TransitionProgress;

public:
	AnimationStateMachine(States startingState) :
		m_CurrentState(startingState),
		m_NextState(startingState),
		m_IsTransitioningAnimationStates(false),
		m_TransitionProgress(0.0f)
	{
		for (int i = 0; i < StateCount; i++)
		{
			m_AnimationProgress[i] = 0.0f;
		}
	}

	~AnimationStateMachine()
	{
	}

	void SetAnimationProgress(int i, float progress)
	{
		m_AnimationProgress[i] = progress;
	}

	void Update(RenderParameters& renderParameters, States targetState)
	{
		if (m_CurrentState != targetState || m_TransitionProgress > 0.0f)
		{
			if (!m_IsTransitioningAnimationStates)
			{
				m_IsTransitioningAnimationStates = true;
				m_TransitionProgress = 0.0f;
				m_NextState = targetState;
			}
			else
			{
				if (m_CurrentState == targetState)
				{
					std::swap(m_CurrentState, m_NextState);
					m_TransitionProgress = 1.0f - m_TransitionProgress;
				}

				m_TransitionProgress += renderParameters.frameTime / kAnimationTransitionLength;

				if (m_TransitionProgress >= 1.0f)
				{
					m_IsTransitioningAnimationStates = false;
					m_TransitionProgress = 0.0f;
					m_CurrentState = targetState;
				}
			}

			renderParameters.targetAnimationState = targetState;
			renderParameters.targetStateAnimationProgress = m_AnimationProgress[targetState] - floor(m_AnimationProgress[targetState]);
			renderParameters.transitionProgress = m_TransitionProgress;
		}
		else
		{
			m_IsTransitioningAnimationStates = false;

			if (kDoesAnimationLoop[m_CurrentState])
			{
				m_AnimationProgress[m_CurrentState] += renderParameters.frameTime / kAnimationLengths[m_CurrentState];
			}
			else if (m_AnimationProgress[m_CurrentState] < 1.0f)
			{
				m_AnimationProgress[m_CurrentState] += renderParameters.frameTime / kAnimationLengths[m_CurrentState];

				if (m_AnimationProgress[m_CurrentState] > 1.0f)
				{
					m_AnimationProgress[m_CurrentState] = 1.0f;
				}
			}
		}

		renderParameters.isTransitioningAnimationStates = m_IsTransitioningAnimationStates;
		renderParameters.currentStateAnimationProgress = m_AnimationProgress[m_CurrentState] - floor(m_AnimationProgress[m_CurrentState]);
		renderParameters.currentAnimationState = m_CurrentState;
	}
};

#undef ANIMATION_STATE_MACHINE_TEMPLATE_PARAMETERS