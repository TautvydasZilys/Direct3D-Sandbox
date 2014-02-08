#include "PrecompiledHeader.h"
#include "FPSControllerCamera.h"
#include "Source\Core\Constants.h"
#include "Source\Core\Parameters.h"
#include "Source\Core\Tools.h"

FPSControllerCamera::FPSControllerCamera(bool usePerspective, float fovY, float aspectRatio, float orthoWidth, float orthoHeight, float jumpHeight) :
	Camera(usePerspective, fovY, aspectRatio, orthoWidth, orthoHeight),
	m_IsJumping(false),
	m_ShouldStartJump(false),
	m_JumpHeight(jumpHeight),
	m_Velocity(0.0f, 0.0f, 0.0f),
	m_LastPosition(m_Position)
{
	Assert(m_JumpHeight > 0);
}

FPSControllerCamera::~FPSControllerCamera()
{
}

void FPSControllerCamera::GoForward(float value)
{
	if (m_IsJumping)
	{
		return;
	}

	m_Position.x -= value * sin(m_Rotation.y);
	m_Position.z -= value * cos(m_Rotation.y);
}

void FPSControllerCamera::GoBack(float value)
{
	if (m_IsJumping)
	{
		return;
	}

	m_Position.x += value * sin(m_Rotation.y);
	m_Position.z += value * cos(m_Rotation.y);
}

void FPSControllerCamera::GoUp(float value)
{
	if (m_IsJumping)
	{
		return;
	}

	m_ShouldStartJump = true;
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
	if (m_IsJumping)
	{
		return;
	}

	m_Position.x -= value * cos(m_Rotation.y);
	m_Position.z -= -value * sin(m_Rotation.y);
}

void FPSControllerCamera::GoRight(float value)
{
	if (m_IsJumping)
	{
		return;
	}

	m_Position.x += value * cos(m_Rotation.y);
	m_Position.z += -value * sin(m_Rotation.y);
}

void FPSControllerCamera::SetRenderParameters(RenderParameters& renderParameters)
{
	if (m_ShouldStartJump)
	{
		m_ShouldStartJump = false;
		m_IsJumping = true;
		
		m_OriginalY = m_Position.y;

		m_Velocity.x = (m_Position.x - m_LastPosition.x) / renderParameters.frameTime;
		m_Velocity.z = (m_Position.z - m_LastPosition.z) / renderParameters.frameTime;
		
		// m_JumpHeight == h - h0
		// h == h0 + v0 * t - g * t^2
		// h - h0 == v0 * t - g * t^2
		// m_JumpHeight == v0 * t - g * t^2
		//
		// v0 + g * t == 0	// Vertical velocity at heighest point is zero
		// t == -v0 / g
		//
		// m_JumpHeight == v0 * (-v0 / g) - g * (v0^2 / g^2)
		// m_JumpHeight == -v0^2 / g - v0^2 / g
		// m_JumpHeight == -2 * v0^2 / g
		// v0^2 = m_JumpHeight * g / (-2)
		// v0 = sqrt(m_JumpHeight * g / (-2))
		m_Velocity.y = sqrt(m_JumpHeight * Constants::GravityConstant / (-2));
	}

	if (m_IsJumping)
	{
		m_Position.x += m_Velocity.x * renderParameters.frameTime;
		m_Position.y += m_Velocity.y * renderParameters.frameTime;
		m_Position.z += m_Velocity.z * renderParameters.frameTime;
		m_Velocity.y += Constants::GravityConstant * renderParameters.frameTime;
		
		Assert(m_Position.x == m_Position.x);
		Assert(m_Position.z == m_Position.z);

		if (m_Position.y < m_OriginalY)
		{
			m_Position.y = m_OriginalY;
			m_IsJumping = false;
		}
	}
	
	if (!m_IsJumping)
	{
		m_LastPosition = m_Position;
	}
	
	Camera::SetRenderParameters(renderParameters);
}