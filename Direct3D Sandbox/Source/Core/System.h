#pragma once

#include "Camera.h"
#include "DirectionalLight.h"
#include "Input.h"
#include "ModelInstance.h"
#include "Source\Graphics\Direct3D.h"
#include "Source\PlatformSpecific\Windows\DesktopWindowing.h"
#include "Source\PlatformSpecific\WindowsPhone\PhoneWindowing.h"

class System
{
private:
	Windowing m_Windowing;

	Direct3D m_Direct3D;
	Input& m_Input;
	float m_MouseSensitivity;

	float m_DeltaTime;
	float m_CurrentTime;
	float m_LastFpsTime;
	float m_Fps;

	Camera m_Camera;
	DirectionalLight m_Light;
	vector<ModelInstance> m_Models;

	void Update();
	void Draw();
	void IncrementFpsCounter();
public:
	System();
	~System();

	void Run();
};

