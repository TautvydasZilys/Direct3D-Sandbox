#pragma once

#include "ZombieInstanceBase.h"

class PlayerInstance;
class ZombieInstance :
	public ZombieInstanceBase
{
	float m_AnimationProgress;
	const vector<weak_ptr<ZombieInstanceBase>>& m_Zombies;
	
	static const float kIdleAnimationPeriod;
	static const float kRunningAnimationPeriod;
	static const float kZombieDistancePerAnimationTime;

	static bool CanMoveTo(const DirectX::XMFLOAT2& position, const vector<weak_ptr<ZombieInstanceBase>>& zombies,
		const ZombieInstanceBase* thisPtr);

	ZombieInstance(const ModelInstance& other);					// Not implemented (no copying allowed)
	ZombieInstance& operator=(const ModelInstance& other);		// Not implemented (no copying allowed)

	enum ZombieStates
	{
		Idle,
		Running
	};

public:
	ZombieInstance(const ModelParameters& modelParameters, const PlayerInstance& targetPlayer, 
		const vector<weak_ptr<ZombieInstanceBase>>& zombies);
	virtual ~ZombieInstance();

	virtual void UpdateAndRender(RenderParameters& renderParameters);

	static shared_ptr<ZombieInstanceBase> Spawn(const PlayerInstance& targetPlayer, const vector<weak_ptr<ZombieInstanceBase>>& zombies);
};

