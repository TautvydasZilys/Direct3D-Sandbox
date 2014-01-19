#include "PrecompiledHeader.h"
#include "System.h"
#include "Tools.h"

System::System() :
	m_Input(Input::GetInstance()),
	m_Direct3D(m_Windowing.GetWindowHandle(), m_Windowing.GetWidth(), m_Windowing.GetHeight(), m_Windowing.IsFullscreen()),
	m_CurrentTime(Tools::GetTime()),
	m_Fps(0), 
	m_LastFpsTime(m_CurrentTime)
{
}

System::~System()
{
}

void System::Run()
{
	while (!m_Input.ShouldQuit())
	{
		m_CurrentTime = Tools::GetTime();
		m_Windowing.DispatchMessages();

		IncrementFpsCounter();
	}
}

void System::IncrementFpsCounter()
{
	m_Fps++;
	
	if (m_CurrentTime - m_LastFpsTime > 1.0f)
	{
		OutputDebugStringW((L"FPS: " + to_wstring(m_Fps) + L"\r\n").c_str());

		m_Fps = 0;
		m_LastFpsTime = m_CurrentTime;
	}
}