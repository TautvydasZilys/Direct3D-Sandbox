#include "PrecompiledHeader.h"
#include "FPSControllerCamera.h"


FPSControllerCamera::FPSControllerCamera(bool usePerspective, float fovY, float aspectRatio, float orthoWidth, float orthoHeight) :
	Camera(usePerspective, fovY, aspectRatio, orthoWidth, orthoHeight)
{
}


FPSControllerCamera::~FPSControllerCamera()
{
}

void FPSControllerCamera::GoForward(float value)
{
	m_Position.x -= value * sin(m_Rotation.y);
	m_Position.z -= value * cos(m_Rotation.y);
}

void FPSControllerCamera::GoBack(float value)
{
	m_Position.x += value * sin(m_Rotation.y);
	m_Position.z += value * cos(m_Rotation.y);
}

void FPSControllerCamera::GoUp(float value)
{
}

void FPSControllerCamera::GoDown(float value)
{
}

// Rotation matrix for only y rotation:
// 
//  cos(y)      0      sin(y)
//   0          0        0
// -sin(y)      0      cos(y)
//
void FPSControllerCamera::GoLeft(float value)
{
	m_Position.x -= value * cos(m_Rotation.y);
	m_Position.z -= -value * sin(m_Rotation.y);
}

void FPSControllerCamera::GoRight(float value)
{
	m_Position.x += value * cos(m_Rotation.y);
	m_Position.z += -value * sin(m_Rotation.y);
}