#pragma once

#include "DirectionalLight.h"
#include "Input.h"
#include "Source\Graphics\Direct3D.h"
#include "Source\Models\ModelInstance.h"
#include "Source\PlatformSpecific\Windows\DesktopWindowing.h"
#include "Source\PlatformSpecific\WindowsPhone\PhoneWindowing.h"

class Camera;

class System
{
private:
	Windowing m_Windowing;

	Direct3D m_Direct3D;
	Input& m_Input;
	float m_MouseSensitivity;

	float m_FrameTime;
	float m_CurrentTime;
	float m_LastFpsTime;
	float m_Fps;

	unique_ptr<Camera> m_Camera;
	DirectionalLight m_Light;
	vector<shared_ptr<ModelInstance>> m_Models;
	
	void Update();
	void Draw();
	void IncrementFpsCounter();

	void UpdateInput();
public:
	System();
	~System();

	void Run();
};

