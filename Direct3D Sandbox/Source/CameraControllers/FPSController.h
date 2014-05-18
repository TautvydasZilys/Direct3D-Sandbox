#pragma once

#include "BaseCameraController.h"
#include "Camera.h"

struct RenderParameters;

class FPSController :
	public BaseCameraController
{
private:
	bool m_IsJumping;
	float m_OriginalY;
	DirectX::XMFLOAT3 m_Velocity;
	DirectX::XMFLOAT3 m_LastPosition;

	static float kJumpHeight;

public:
	FPSController(Camera& camera);
	~FPSController();
	
	template <typename CanMoveToFunctor>
	void Update(float frameTime, CanMoveToFunctor canMoveTo);

	inline const DirectX::XMFLOAT3& GetPosition() const { return m_Camera.GetPosition(); }
	inline const DirectX::XMFLOAT3& GetRotation() const { return m_Camera.GetRotation(); }
};


template <typename CanMoveToFunctor>
void FPSController::Update(float frameTime, CanMoveToFunctor canMoveTo)
{
	auto& input = Input::GetInstance();
	auto cameraPosition = m_Camera.GetPosition();
	
	if (input.IsKeyDown(VK_SPACE) && !m_IsJumping)
	{
		m_IsJumping = true;
		
		m_OriginalY = cameraPosition.y;

		m_Velocity.x = (cameraPosition.x - m_LastPosition.x) / frameTime;
		m_Velocity.z = (cameraPosition.z - m_LastPosition.z) / frameTime;
		
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
		m_Velocity.y = sqrt(kJumpHeight * Constants::GravityConstant / (-2));
	}
	
	UpdateLookAround(frameTime);
	auto lastPosition = m_Camera.GetPosition();

	if (m_IsJumping)
	{
		DirectX::XMFLOAT2 newPosition(cameraPosition.x + m_Velocity.x * frameTime, cameraPosition.z + m_Velocity.z * frameTime);
		
		Assert(newPosition.x == newPosition.x);
		Assert(newPosition.y == newPosition.y);

		if (canMoveTo(newPosition))
		{
			cameraPosition.x = newPosition.x;
			cameraPosition.z = newPosition.y;
		}

		cameraPosition.y += m_Velocity.y * frameTime;
		
		m_Velocity.y += Constants::GravityConstant * frameTime;

		if (cameraPosition.y < m_OriginalY)
		{
			cameraPosition.y = m_OriginalY;
			m_IsJumping = false;
		}

		m_Camera.SetPosition(cameraPosition);	// Set to ensure y coordinate change even if subsequent not jumping block cannot move
	}

	if (!m_IsJumping)
	{
		auto deltaX = GetInputX();
		auto deltaZ = GetInputZ();
		
		if (deltaX != 0.0f || deltaZ != 0.0f)
		{
			auto length = sqrt(deltaX * deltaX + deltaZ * deltaZ);
		
			auto multiplier = 5.0f * frameTime / length;

			deltaX *= multiplier;
			deltaZ *= multiplier;
		}
		
		if (input.IsKeyDown(VK_OEM_3))
		{
			deltaZ -= 100.0f * frameTime;
		}

		deltaZ -= input.HandlePinchDisplacement() / 100.0f;

		if (deltaX != 0.0f || deltaZ != 0.0f)
		{
			auto cameraRotation = m_Camera.GetRotation();
			auto yRotationCos = cos(cameraRotation.y);
			auto yRotationSin = sin(cameraRotation.y);

			cameraPosition.x += deltaX * yRotationCos;
			cameraPosition.z -= deltaX * yRotationSin;

			cameraPosition.x += deltaZ * yRotationSin;
			cameraPosition.z += deltaZ * yRotationCos;

			if (canMoveTo(DirectX::XMFLOAT2(cameraPosition.x, cameraPosition.z)))
			{
				m_Camera.SetPosition(cameraPosition);
			}
		}
	}
	
	UpdateSoundListener(frameTime, m_LastPosition);
	m_LastPosition = lastPosition;
}