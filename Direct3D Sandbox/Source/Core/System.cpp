#include "PrecompiledHeader.h"
#include "Constants.h"
#include "Source\Graphics\IShader.h"
#include "System.h"
#include "Tools.h"

System::System() :
	m_Input(Input::GetInstance()),
	m_Direct3D(m_Windowing.GetWindowHandle(), m_Windowing.GetWidth(), m_Windowing.GetHeight(), m_Windowing.IsFullscreen()),
	m_CurrentTime(Tools::GetTime()),
	m_Fps(0), 
	m_LastFpsTime(m_CurrentTime),
	m_Camera(true, Constants::VerticalFieldOfView, m_Windowing.GetAspectRatio(), 0, 0)
{
	IShader::LoadShaders(m_Direct3D.GetDevice());

	for (const auto& model : Tools::GetFilesInDirectory(L"Assets\\Models", L"*.obj", true))
	{
		Model::InitializeModel(m_Direct3D.GetDevice(), IShader::GetShader(ShaderType::COLOR_SHADER), model);
	}
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

		Update();
		Draw();

		IncrementFpsCounter();
	}
}

void System::Update()
{
	if (m_Input.IsKeyDown(VK_ESCAPE))
	{
		m_Input.Quit();
	}
}

void System::Draw()
{
	m_Direct3D.StartDrawing(1.0f);
	m_Direct3D.SwapBuffers();
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