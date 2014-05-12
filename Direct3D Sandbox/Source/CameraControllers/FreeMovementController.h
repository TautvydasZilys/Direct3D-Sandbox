#pragma once

#include "BaseCameraController.h"

struct RenderParameters;

class FreeMovementController : 
	public BaseCameraController
{
public:
	FreeMovementController(Camera& camera);
	~FreeMovementController();

	void Update(float frameTime);
};

