#include "PrecompiledHeader.h"
#include "Constants.h"
#include "Source\Graphics\IShader.h"
#include "Source\Graphics\SamplerState.h"
#include "Source\Graphics\Texture.h"
#include "Source\Models\InfiniteGroundModelInstance.h"
#include "System.h"
#include "Tools.h"

System::System() :
	m_Input(Input::GetInstance()),
	m_Direct3D(m_Windowing.GetWindowHandle(), m_Windowing.GetWidth(), m_Windowing.GetHeight(), m_Windowing.IsFullscreen()),
	m_CurrentTime(Tools::GetTime()),
	m_Fps(0), 
	m_LastFpsTime(m_CurrentTime),
	m_MouseSensitivity(Constants::DefaultMouseSensitivity),
	m_Camera(true, Constants::VerticalFieldOfView, m_Windowing.GetAspectRatio(), 0, 0),
	m_Light(DirectX::XMFLOAT3(3.0f, -2.0f, -1.0f), DirectX::XMFLOAT3(0.7f, 0.7f, 0.6f), DirectX::XMFLOAT3(0.4f, 0.4f, 0.4f), 32)  
{
	// Initialize sample states
	SamplerState::Initialize();

	// Load shaders
	IShader::LoadShaders();

	// Load textures
	for (const auto& texture : Tools::GetFilesInDirectory(L"Assets\\Textures", L"*.dds", true))
	{
		Texture::LoadTexture(texture);
	}

	// Create scene

	auto& playgroundShader = IShader::GetShader(ShaderType::PLAYGROUND_SHADER);
	auto& textureShader = IShader::GetShader(ShaderType::TEXTURE_SHADER);

	ModelParameters modelParameters; 
	
	modelParameters.position = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	modelParameters.rotation = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	modelParameters.scale = DirectX::XMFLOAT3(5000.0f, 5000.0f, 5000.0f);
	modelParameters.color = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	m_Models.emplace_back(make_shared<CameraPositionLockedModelInstance>(textureShader, L"Assets\\Models\\skybox.model", modelParameters, 
		L"Assets\\Textures\\Skybox.dds", TypedDimensions<bool>(true, true, true)));
	
	modelParameters.scale = DirectX::XMFLOAT3(4000.0f, 4000.0f, 4000.0f);
	m_Models.emplace_back(make_shared<InfiniteGroundModelInstance>(modelParameters, L"Assets\\Textures\\Grass.dds", DirectX::XMFLOAT2(5000.0f, 5000.0f)));

	m_Camera.SetPosition(0.0f, 1.0f, 0.0f);
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
	UpdateInput();

	wstringstream debugStream;
	debugStream.setf(ios::fixed);
	debugStream.precision(3);

	auto& cameraPosition = m_Camera.GetPosition();
	debugStream << L"Camera position: " << cameraPosition.x << L" " << cameraPosition.y << L" " << cameraPosition.z << endl;

	OutputDebugStringW(debugStream.str().c_str());
}

void System::UpdateInput()
{
	if (m_Input.IsKeyDown(VK_ESCAPE))
	{
		m_Input.Quit();
	}
	
	m_Camera.GoForward(m_Input.HandlePinchDisplacement() / 100.0f);

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
		m_Camera.LookRight(m_MouseSensitivity * mouseX / 250.0f);
	}
	if (mouseY > 0.000001f || mouseY < -0.000001f)
	{
		m_Camera.LookDown(m_MouseSensitivity * mouseY / 250.0f);
	}

	if (m_Input.IsKeyDown(VK_ADD))
	{
		m_MouseSensitivity += m_DeltaTime * m_MouseSensitivity;
	}
	if (m_Input.IsKeyDown(VK_SUBTRACT))
	{
		m_MouseSensitivity -= m_DeltaTime * m_MouseSensitivity;
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
	m_Light.SetRenderParameters(renderParameters);
	
	for (auto& model : m_Models)
	{
		model->Render(renderParameters);
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