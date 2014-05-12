#include "PrecompiledHeader.h"
#include "BaseCameraController.h"
#include "Camera.h"
#include "Input.h"
#include "System.h"

BaseCameraController::BaseCameraController(Camera& camera) :
	m_Camera(camera)
{
}

BaseCameraController::~BaseCameraController()
{
}

float BaseCameraController::GetInputX()
{
	auto& input = Input::GetInstance();
	float x = 0.0f;
	
	if (input.IsKeyDown('A'))
	{
		x -= 1.0f;
	}

	if (input.IsKeyDown('D'))
	{
		x += 1.0f;
	}

	return x;
}

float BaseCameraController::GetInputZ()
{
	auto& input = Input::GetInstance();
	float z = 0.0f;
	
	if (input.IsKeyDown('W'))
	{
		z -= 1.0f;
	}

	if (input.IsKeyDown('S'))
	{
		z += 1.0f;
	}

	return z;
}

void BaseCameraController::UpdateLookAround(float frameTime)
{
	auto& input = Input::GetInstance();
	auto deltaUp = 0.0f;
	auto deltaRight = 0.0f;
	
	if (input.IsKeyDown(VK_UP))
	{
		deltaUp += frameTime / 2.0f;
	}
	if (input.IsKeyDown(VK_DOWN))
	{
		deltaUp -= frameTime / 2.0f;
	}
	if (input.IsKeyDown(VK_LEFT))
	{
		deltaRight -= frameTime / 2.0f;
	}
	if (input.IsKeyDown(VK_RIGHT))
	{
		deltaRight += frameTime / 2.0f;
	}
	
	long mouseX, mouseY;
	auto mouseSensitivity = System::GetInstance().GetMouseSensitivity();

	input.HandleMouseDisplacement(mouseX, mouseY);

	if (mouseX > 0.000001f || mouseX < -0.000001f)
	{
		deltaRight += mouseSensitivity * mouseX / 250.0f;
	}

	if (mouseY > 0.000001f || mouseY < -0.000001f)
	{
		deltaUp -= mouseSensitivity * mouseY / 250.0f;
	}
	
	auto cameraRotation = m_Camera.GetRotation();

	// Up
	cameraRotation.x += deltaUp;

	if (cameraRotation.x > DirectX::XM_PI / 2.0f)
	{
		cameraRotation.x = DirectX::XM_PI / 2.0f;
	}
	else if (cameraRotation.x < -DirectX::XM_PI / 2.0f)
	{
		cameraRotation.x = -DirectX::XM_PI / 2.0f;
	}
	
	// Right
	cameraRotation.y -= deltaRight;

	m_Camera.SetRotation(cameraRotation);
}