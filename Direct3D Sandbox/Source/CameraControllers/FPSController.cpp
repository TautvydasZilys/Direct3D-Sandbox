#include "PrecompiledHeader.h"
#include "Camera.h"
#include "Constants.h"
#include "FPSController.h"
#include "Input.h"
#include "Parameters.h"
#include "Tools.h"

float FPSController::kJumpHeight = 2.0f;

FPSController::FPSController(Camera& camera) :
	BaseCameraController(camera),
	m_IsJumping(false),
	m_Velocity(0.0f, 0.0f, 0.0f),
	m_LastPosition(camera.GetPosition())
{
}

FPSController::~FPSController()
{
}
