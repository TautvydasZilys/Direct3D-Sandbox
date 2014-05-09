#pragma once

#include "ZombieInstanceBase.h"

class PlayerInstance;
class ZombieInstance :
	public ZombieInstanceBase
{
	const vector<weak_ptr<ZombieInstanceBase>>& m_Zombies;
	
	enum ZombieStates
	{
		Idle = 0,
		Running,
		Hitting,
		Death,
		StateCount
	};
	
	static const float kAnimationPeriods[ZombieStates::StateCount];
	static const bool kDoesAnimationLoop[ZombieStates::StateCount];
	static const float kZombieDistancePerRunningAnimationTime;
	static const float kZombieBodyLastingTime;
	
	ZombieStates m_CurrentState;
	float m_AnimationProgress[ZombieStates::StateCount];
	bool m_IsTransitioningAnimationStates;
	float m_TransitionProgress;

	float m_GonnaLiveFor;

	static float GetAnimationTransitionLength(ZombieStates from, ZombieStates to);
	static bool CanMoveTo(const DirectX::XMFLOAT2& position, const vector<weak_ptr<ZombieInstanceBase>>& zombies,
		const ZombieInstanceBase* thisPtr);

	void SetAnimationParameters(RenderParameters& renderParameters, ZombieStates targetState);

	ZombieInstance(const ModelInstance& other);					// Not implemented (no copying allowed)
	ZombieInstance& operator=(const ModelInstance& other);		// Not implemented (no copying allowed)

public:
	ZombieInstance(const ModelParameters& modelParameters, const PlayerInstance& targetPlayer, 
		const vector<weak_ptr<ZombieInstanceBase>>& zombies);
	virtual ~ZombieInstance();

	virtual void UpdateAndRender(RenderParameters& renderParameters);

	static shared_ptr<ZombieInstanceBase> Spawn(const PlayerInstance& targetPlayer, const vector<weak_ptr<ZombieInstanceBase>>& zombies);
};

