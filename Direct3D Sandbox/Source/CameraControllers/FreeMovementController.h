#pragma once

#include "BaseCameraController.h"

struct RenderParameters;

class FreeMovementController : 
	public BaseCameraController
{
private:
	DirectX::XMFLOAT3 m_LastPosition;

public:
	FreeMovementController(Camera& camera);
	~FreeMovementController();

	void Update(float frameTime);
};

