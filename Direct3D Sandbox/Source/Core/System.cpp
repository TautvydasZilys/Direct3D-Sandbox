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

	auto& colorShader = IShader::GetShader(ShaderType::COLOR_SHADER);
	ModelParameters modelParameters; 

	modelParameters.position = DirectX::XMFLOAT3(0.0f, 0.0f, -30.0f);
	modelParameters.rotation = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	modelParameters.scale = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);	
	modelParameters.color = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	m_Models.emplace_back(m_Direct3D.GetDevice(), colorShader, L"Assets\\Models\\Cube.obj", modelParameters);
}

System::~System()
{
}

void System::Run()
{
	while (!m_Input.ShouldQuit())
	{
		m_DeltaTime = Tools::GetTime() - m_CurrentTime;
		m_CurrentTime += m_DeltaTime;
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
	
	if (m_Input.IsKeyDown(VK_OEM_3))
	{
		m_Camera.GoForward(100.0f * m_DeltaTime);
	}

	if (m_Input.IsKeyDown('W'))
	{
		m_Camera.GoForward(5.0f * m_DeltaTime);
	}
	if (m_Input.IsKeyDown('S'))
	{
		m_Camera.GoBack(5.0f * m_DeltaTime);
	}
	if (m_Input.IsKeyDown('A'))
	{
		m_Camera.GoLeft(5.0f * m_DeltaTime);
	}
	if (m_Input.IsKeyDown('D'))
	{
		m_Camera.GoRight(5.0f * m_DeltaTime);
	}	
	if (m_Input.IsKeyDown(VK_SPACE))
	{
		m_Camera.GoUp(5.0f * m_DeltaTime);
	}
	if (m_Input.IsKeyDown('X'))
	{
		m_Camera.GoDown(5.0f * m_DeltaTime);
	}
	
	if (m_Input.IsKeyDown(VK_UP))
	{
		m_Camera.LookUp(m_DeltaTime / 2.0f);
	}
	if (m_Input.IsKeyDown(VK_DOWN))
	{
		m_Camera.LookDown(m_DeltaTime / 2.0f);
	}
	if (m_Input.IsKeyDown(VK_LEFT))
	{
		m_Camera.LookLeft(m_DeltaTime / 2.0f);
	}
	if (m_Input.IsKeyDown(VK_RIGHT))
	{
		m_Camera.LookRight(m_DeltaTime / 2.0f);
	}
	
	long mouseX, mouseY;
	m_Input.HandleMouseDisplacement(mouseX, mouseY);
	if (mouseX > 0.000001f || mouseX < -0.000001f)
	{
		m_Camera.LookRight(mouseX / 1000.0f);
	}
	if (mouseY > 0.000001f || mouseY < -0.000001f)
	{
		m_Camera.LookDown(mouseY / 1000.0f);
	}
}

void System::Draw()
{
	RenderParameters renderParameters;

	m_Direct3D.SetBackBufferAsRenderTarget();
	m_Direct3D.TurnZBufferOn();
	m_Direct3D.StartDrawing();

	renderParameters.time = m_CurrentTime;
	m_Camera.SetRenderParameters(renderParameters);
	
	for (auto& model : m_Models)
	{
		model.Render(m_Direct3D.GetDeviceContext(), renderParameters);
	}

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