#pragma once

class Camera;
class BaseCameraController
{
protected:
	Camera& m_Camera;
	float m_CameraStartingHeight;

	BaseCameraController(Camera& camera);
	
	float GetInputX();
	float GetInputZ();

	void UpdateLookAround(float frameTime);
	void UpdateSoundListener(float frameTime, const DirectX::XMFLOAT3& lastPosition);

public:
	~BaseCameraController();
};

