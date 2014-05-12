#pragma once

class Camera;
class BaseCameraController
{
protected:
	Camera& m_Camera;

	BaseCameraController(Camera& camera);
	
	float GetInputX();
	float GetInputZ();

	void UpdateLookAround(float frameTime);

public:
	~BaseCameraController();
};

