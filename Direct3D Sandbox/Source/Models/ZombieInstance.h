#pragma once

#include "AnimationStateMachine.h"
#include "Source\Audio\AudioEmitter.h"
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
	static const float kZombieHitInterval;
	static const float kNearPlayerSoundInterval;
	
	const vector<shared_ptr<ZombieInstanceBase>>& m_Zombies;
	float m_LastHitPlayerAt;
	float m_LastMadeNearPlayerSound;
	AudioEmitter m_AudioEmitter;
	Sound& m_NearPlayerSound;

	AnimationStateMachine<ZombieStates, ZombieStates::StateCount, kAnimationPeriods, kDoesAnimationLoop, kAnimationTransitionLength> m_AnimationStateMachine;

	ZombieInstance(const ModelInstance& other);					// Not implemented (no copying allowed)
	ZombieInstance& operator=(const ModelInstance& other);		// Not implemented (no copying allowed)
	
	ZombieInstance(const ModelParameters& modelParameters, PlayerInstance& targetPlayer, 
		const vector<shared_ptr<ZombieInstanceBase>>& zombies);

public:
	virtual ~ZombieInstance();
	
	virtual void Update(const RenderParameters& renderParameters);
	virtual void Render3D(RenderParameters& renderParameters);
	
	static bool CanMoveTo(const DirectX::XMFLOAT2& position, const vector<shared_ptr<ZombieInstanceBase>>& zombies,
		const ZombieInstanceBase* thisPtr);
	static shared_ptr<ZombieInstanceBase> Spawn(PlayerInstance& targetPlayer, const vector<shared_ptr<ZombieInstanceBase>>& zombies);
};

