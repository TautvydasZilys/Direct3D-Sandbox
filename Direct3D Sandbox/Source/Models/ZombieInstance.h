#pragma once

#include "ModelInstance.h"

class PlayerInstance;
class ZombieInstance :
	public ModelInstance
{	
	const PlayerInstance& m_TargetPlayer;

	ZombieInstance(const ModelInstance& other);					// Not implemented (no copying allowed)
	ZombieInstance& operator=(const ModelInstance& other);		// Not implemented (no copying allowed)

public:
	ZombieInstance(const ModelParameters& modelParameters, const PlayerInstance& targetPlayer);
	virtual ~ZombieInstance();

	virtual void UpdateAndRender(RenderParameters& renderParameters);

	static shared_ptr<ZombieInstance> Spawn(const PlayerInstance& targetPlayer);
};

