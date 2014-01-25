#pragma once

#include "Source\Graphics\Direct3D.h"
#include "Source\PlatformSpecific\Windows\DesktopWindowing.h"
#include "Source\PlatformSpecific\WindowsPhone\PhoneWindowing.h"
#include "Input.h"

class System
{
private:
	Windowing m_Windowing;

	Direct3D m_Direct3D;
	Input& m_Input;

	float m_CurrentTime;
	float m_LastFpsTime;
	float m_Fps;
	
	void Update();
	void Draw();
	void IncrementFpsCounter();
public:
	System();
	~System();

	void Run();
};

