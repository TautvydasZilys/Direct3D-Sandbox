#pragma once

#include "Camera.h"

class FPSControllerCamera :
	public Camera
{
private:
	bool m_IsJumping;
	bool m_ShouldStartJump;
	float m_JumpHeight;
	float m_OriginalY;
	DirectX::XMFLOAT3 m_Velocity;
	DirectX::XMFLOAT3 m_LastPosition;

public:
	FPSControllerCamera(bool usePerspective, float fovY, float aspectRatio, float orthoWidth, float orthoHeight, float jumpHeight);
	virtual ~FPSControllerCamera();
	
	virtual void GoForward(float value);
	virtual void GoBack(float value);
	virtual void GoUp(float value);
	virtual void GoDown(float value);
	virtual void GoLeft(float value);
	virtual void GoRight(float value);

	virtual void SetRenderParameters(RenderParameters& renderParameters);
};