#pragma once

#include "AnimationStateMachine.h"
#include "ZombieInstanceBase.h"

class PlayerInstance;
class ZombieInstance :
	public ZombieInstanceBase
{	
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
	static const float kAnimationTransitionLength;
	static const float kZombieDistancePerRunningAnimationTime;
	static const float kZombieBodyLastingTime;
	
	const vector<weak_ptr<ZombieInstanceBase>>& m_Zombies;

	AnimationStateMachine<ZombieStates, ZombieStates::StateCount, kAnimationPeriods, kDoesAnimationLoop, kAnimationTransitionLength> m_AnimationStateMachine;
	float m_GonnaLiveFor;

	static bool CanMoveTo(const DirectX::XMFLOAT2& position, const vector<weak_ptr<ZombieInstanceBase>>& zombies,
		const ZombieInstanceBase* thisPtr);

	ZombieInstance(const ModelInstance& other);					// Not implemented (no copying allowed)
	ZombieInstance& operator=(const ModelInstance& other);		// Not implemented (no copying allowed)

public:
	ZombieInstance(const ModelParameters& modelParameters, const PlayerInstance& targetPlayer, 
		const vector<weak_ptr<ZombieInstanceBase>>& zombies);
	virtual ~ZombieInstance();

	virtual void UpdateAndRender3D(RenderParameters& renderParameters);

	static shared_ptr<ZombieInstanceBase> Spawn(const PlayerInstance& targetPlayer, const vector<weak_ptr<ZombieInstanceBase>>& zombies);
};

