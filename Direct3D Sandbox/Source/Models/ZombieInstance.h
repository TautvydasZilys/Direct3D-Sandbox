#pragma once

#include "ModelInstance.h"

class ZombieInstance :
	public ModelInstance
{
	
	ZombieInstance(const ModelInstance& other);					// Not implemented (no copying allowed)
	ZombieInstance& operator=(const ModelInstance& other);		// Not implemented (no copying allowed)

public:
	ZombieInstance(const ModelParameters& modelParameters);
	virtual ~ZombieInstance();

	static shared_ptr<ZombieInstance> Spawn(const DirectX::XMFLOAT3& playerPosition);
};

