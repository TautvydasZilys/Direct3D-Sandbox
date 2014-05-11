#pragma once

#include "Parameters.h"

#define ANIMATION_STATE_MACHINE_TEMPLATE_PARAMETERS \
	typename States, int StateCount, const float kAnimationLengths[StateCount], const bool kDoesAnimationLoop[StateCount], const float& kAnimationTransitionLength

template <ANIMATION_STATE_MACHINE_TEMPLATE_PARAMETERS>
class AnimationStateMachine
{
	States m_CurrentState;
	States m_NextState;
	States m_TargetState;
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

	inline void SetAnimationProgress(int i, float progress)
	{
		m_AnimationProgress[i] = progress;
	}

	inline void Update(const RenderParameters& renderParameters, States targetState)
	{
		m_TargetState = targetState;

		if (m_CurrentState != m_TargetState || m_TransitionProgress > 0.0f)
		{
			if (!m_IsTransitioningAnimationStates)
			{
				m_IsTransitioningAnimationStates = true;
				m_TransitionProgress = 0.0f;
				m_NextState = m_TargetState;
			}
			else
			{
				if (m_CurrentState == m_TargetState)
				{
					std::swap(m_CurrentState, m_NextState);
					m_TransitionProgress = 1.0f - m_TransitionProgress;
				}

				m_TransitionProgress += renderParameters.frameTime / kAnimationTransitionLength;

				if (m_TransitionProgress >= 1.0f)
				{
					m_IsTransitioningAnimationStates = false;
					m_TransitionProgress = 0.0f;
					m_CurrentState = m_TargetState;
				}
			}
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
	}

	inline void SetRenderParameters(RenderParameters& renderParameters)
	{
		renderParameters.targetAnimationState = m_TargetState;
		renderParameters.targetStateAnimationProgress = m_AnimationProgress[m_TargetState] - floor(m_AnimationProgress[m_TargetState]);
		renderParameters.transitionProgress = m_TransitionProgress;
		
		renderParameters.isTransitioningAnimationStates = m_IsTransitioningAnimationStates;
		renderParameters.currentStateAnimationProgress = m_AnimationProgress[m_CurrentState] - floor(m_AnimationProgress[m_CurrentState]);
		renderParameters.currentAnimationState = m_CurrentState;
	}

	inline bool IsTransitioningAnimationStates() const { return m_IsTransitioningAnimationStates; }
	inline States GetCurrentAnimationState() const { return m_CurrentState; }
	inline float GetCurrentStateAnimationProgress() const { return m_AnimationProgress[m_CurrentState] - floor(m_AnimationProgress[m_CurrentState]); }
};

#undef ANIMATION_STATE_MACHINE_TEMPLATE_PARAMETERS