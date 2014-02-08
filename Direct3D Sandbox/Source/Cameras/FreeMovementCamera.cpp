#include "PrecompiledHeader.h"
#include "FreeMovementCamera.h"


FreeMovementCamera::FreeMovementCamera(bool usePerspective, float fovY, float aspectRatio, float orthoWidth, float orthoHeight) :
	Camera(usePerspective, fovY, aspectRatio, orthoWidth, orthoHeight)
{
}

FreeMovementCamera::~FreeMovementCamera()
{
}

void FreeMovementCamera::GoForward(float value)
{
	m_Position.x -= value * cos(m_Rotation.x) * sin(m_Rotation.y);
	m_Position.y -= -value * sin(m_Rotation.x);
	m_Position.z -= value * cos(m_Rotation.x) * cos(m_Rotation.y);
}

void FreeMovementCamera::GoBack(float value)
{
	m_Position.x += value * cos(m_Rotation.x) * sin(m_Rotation.y);
	m_Position.y += -value * sin(m_Rotation.x);
	m_Position.z += value * cos(m_Rotation.x) * cos(m_Rotation.y);
}

void FreeMovementCamera::GoUp(float value)
{
	m_Position.y += value;
}

void FreeMovementCamera::GoDown(float value)
{
	m_Position.y -= value;
}

// Rotation matrix for only y rotation:
// 
//  cos(y)      0      sin(y)
//   0          0        0
// -sin(y)      0      cos(y)
//
void FreeMovementCamera::GoLeft(float value)
{
	m_Position.x -= value * cos(m_Rotation.y);
	m_Position.z -= -value * sin(m_Rotation.y);
}

void FreeMovementCamera::GoRight(float value)
{
	m_Position.x += value * cos(m_Rotation.y);
	m_Position.z += -value * sin(m_Rotation.y);
}