#pragma once

#include "ZombieInstanceBase.h"

class PlayerInstance;
class ZombieInstance :
	public ZombieInstanceBase
{
	float m_StartingAnimationProgress;
	
	static const float kAnimationPeriod;
	static const float kZombieDistancePerAnimationTime;

	ZombieInstance(const ModelInstance& other);					// Not implemented (no copying allowed)
	ZombieInstance& operator=(const ModelInstance& other);		// Not implemented (no copying allowed)

public:
	ZombieInstance(const ModelParameters& modelParameters, const PlayerInstance& targetPlayer);
	virtual ~ZombieInstance();

	virtual void UpdateAndRender(RenderParameters& renderParameters);

	static shared_ptr<ZombieInstanceBase> Spawn(const PlayerInstance& targetPlayer);
};

