#include "PrecompiledHeader.h"
#include "Camera.h"
#include "FreeMovementController.h"
#include "Input.h"
#include "Parameters.h"


FreeMovementController::FreeMovementController(Camera& camera) :
	BaseCameraController(camera),
	m_LastPosition(camera.GetPosition())
{
}

FreeMovementController::~FreeMovementController()
{
}

void FreeMovementController::Update(float frameTime)
{
	auto& input = Input::GetInstance();

	UpdateLookAround(frameTime);

	auto deltaX = GetInputX();
	auto deltaZ = GetInputZ();
	float deltaY = 0.0f;

	if (input.IsKeyDown('X'))
	{
		deltaY -= 1.0f;
	}

	if (input.IsKeyDown(VK_SPACE))
	{
		deltaY += 1.0f;
	}

	if (deltaX == 0.0f && deltaZ == 0.0f && deltaY == 0.0f)
	{
		return;
	}

	auto length = sqrt(deltaX * deltaX + deltaY * deltaY + deltaZ * deltaZ);
	auto multiplier = 5.0f * frameTime / length;

	auto cameraPosition = m_Camera.GetPosition();
	auto& cameraRotation = m_Camera.GetRotation();

	deltaX *= multiplier;
	deltaY *= multiplier;
	deltaZ *= multiplier;
	
	cameraPosition.x += deltaX * cos(cameraRotation.y);
	cameraPosition.z -= deltaX * sin(cameraRotation.y);
	
	cameraPosition.y += deltaY;

	cameraPosition.x += deltaZ * cos(cameraRotation.x) * sin(cameraRotation.y);
	cameraPosition.y -= deltaZ * sin(cameraRotation.x);
	cameraPosition.z += deltaZ * cos(cameraRotation.x) * cos(cameraRotation.y);
	
	UpdateSoundListener(frameTime, m_LastPosition);
	m_LastPosition = m_Camera.GetPosition();
	m_Camera.SetPosition(cameraPosition);
}