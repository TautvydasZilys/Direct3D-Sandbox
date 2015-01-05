#include "PrecompiledHeader.h"
#include "Constants.h"
#include "Camera.h"
#include "Source\Graphics\Font.h"
#include "Source\Graphics\IShader.h"
#include "Source\Graphics\SamplerState.h"
#include "Source\Graphics\Texture.h"
#include "System.h"
#include "Tools.h"

System* System::s_Instance;

System::System() :
	m_Input(Input::GetInstance()),
	m_Direct3D(m_Windowing.GetWindowHandle(), m_Windowing.GetWidth(), m_Windowing.GetHeight(), m_Windowing.IsFullscreen()),
	m_CurrentTime(Tools::GetTime()),
	m_Fps(0), 
	m_LastFrameFps(0),
	m_LastFpsTime(m_CurrentTime),
	m_MouseSensitivity(Constants::DefaultMouseSensitivity),
	m_Camera(new Camera(true, Constants::VerticalFieldOfView, m_Windowing.GetAspectRatio(), 0, 0)),
	m_OrthoCamera(new Camera(false, 0.0f, 0.0f, static_cast<float>(m_Windowing.GetWidth()), static_cast<float>(m_Windowing.GetHeight()))),
	m_Light(DirectX::XMFLOAT3(3.0f, -2.0f, -1.0f), DirectX::XMFLOAT3(0.4f, 0.2f, 0.2f), DirectX::XMFLOAT3(0.15f, 0.15f, 0.15f), 32)
{
	s_Instance = this;

	// Initialize sample states
	SamplerState::Initialize();

	// Load shaders
	IShader::LoadShaders();
	
	// Load textures
	for (const auto& texture : Tools::GetFilesInDirectory(L"Assets\\Textures", L"*.dds", true))
	{
		Texture::LoadTexture(texture);
	}

	// Load normal maps
	for (const auto& texture : Tools::GetFilesInDirectory(L"Assets\\Normal Maps", L"*.dds", true))
	{
		Texture::LoadTexture(texture);
	}

	// Load fonts
	for (const auto& font : Tools::GetFilesInDirectory(L"Assets\\Fonts", L"*.font", true))
	{
		Font::LoadFont(font);
	}

	Font::SetDefault(L"Assets\\Fonts\\Segoe UI Light.font");

	// Create scene
	
	m_Camera->SetPosition(0.0f, 1.5f, 0.0f);
	m_OrthoCamera->SetPosition(0.0f, 0.0f, 1.0f);


}

System::~System()
{
}

void System::Run()
{
	while (!m_Input.ShouldQuit())
	{
		auto currentTime = Tools::GetTime();
		m_FrameTime = static_cast<float>(currentTime - m_CurrentTime);
		m_CurrentTime = currentTime;
		m_Windowing.DispatchMessages();

		UpdateAndDraw();

		IncrementFpsCounter();

#if DEBUG
		Assert(_CrtCheckMemory() == TRUE);
#endif
	}
}

void System::UpdateAndDraw()
{
	RenderParameters renderParameters;

	renderParameters.time = static_cast<float>(m_CurrentTime);
	renderParameters.frameTime = m_FrameTime;
	renderParameters.screenWidth = m_Windowing.GetWidth();
	renderParameters.screenHeight = m_Windowing.GetHeight();

	Update(renderParameters);
	Draw(renderParameters);
}

void System::Update(const RenderParameters& renderParameters)
{
	AddAndRemoveModels();
	UpdateInput();

	for (auto& model : m_Models)
	{
		model->Update(renderParameters);
	}
}

void System::UpdateInput()
{
	if (m_Input.IsKeyDown(VK_ESCAPE))
	{
		m_Input.Quit();
	}

	if (m_Input.IsKeyDown(VK_ADD))
	{
		m_MouseSensitivity += m_FrameTime * m_MouseSensitivity;
	}
	if (m_Input.IsKeyDown(VK_SUBTRACT))
	{
		m_MouseSensitivity -= m_FrameTime * m_MouseSensitivity;
	}
}

void System::Draw(RenderParameters& renderParameters)
{
	m_Direct3D.StartDrawing();
	
	{
		m_Direct3D.SetBackBufferAsRenderTarget(Eye::Left);
		m_Camera->SetPosition(-0.2f, 1.5f, 0.0f);
		DrawForSingleEye(renderParameters);
	}
	
	{
		m_Direct3D.SetBackBufferAsRenderTarget(Eye::Right);
		m_Camera->SetPosition(0.2f, 1.5f, 0.0f);
		DrawForSingleEye(renderParameters);
	}

	m_Direct3D.SwapBuffers();
}

void System::DrawForSingleEye(RenderParameters& renderParameters)
{
	m_Direct3D.TurnZBufferOn();
	
	m_Camera->SetRenderParameters(renderParameters);
	m_Light.SetRenderParameters(renderParameters);

	for (auto& model : m_Models)
	{
		model->Render3D(renderParameters);
	}

	m_Direct3D.TurnZBufferOff();
	m_OrthoCamera->SetRenderParameters(renderParameters);

	renderParameters.color = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	Font::GetDefault().DrawText("FPS: " + to_string(m_LastFrameFps), 25, 25, renderParameters);
	Font::GetDefault().DrawText("Memory Usage: " + to_string(Tools::GetMemoryUsage()) + " MB", 25, 75, renderParameters, true);

	for (auto& model : m_Models)
	{
		model->Render2D(renderParameters);
	}
}

void System::IncrementFpsCounter()
{
	m_Fps++;
	
	if (m_CurrentTime - m_LastFpsTime > 1.0)
	{
		m_LastFrameFps = m_Fps;
		m_Fps = 0;
		m_LastFpsTime = m_CurrentTime;
	}
}

void System::AddModel(shared_ptr<IModelInstance> model)
{
	AddRemoveModelItem addModel;

	addModel.operation = AddRemoveModelItem::AddRemoveOperation::ADD;
	addModel.modelToAdd = model;

	m_AddRemoveModelQueue.push_back(addModel);
}

void System::AddModelImpl(shared_ptr<IModelInstance> model)
{
	m_Models.push_back(model);
}

void System::RemoveModel(const IModelInstance* model)
{	
	AddRemoveModelItem removeModel;

	removeModel.operation = AddRemoveModelItem::AddRemoveOperation::REMOVE;
	removeModel.modelToRemove = model;

	m_AddRemoveModelQueue.push_back(removeModel);
}

void System::RemoveModelImpl(const IModelInstance* model)
{
	for (auto i = 0u; i < m_Models.size(); i++)
	{
		if (m_Models[i].get() == model)
		{
			m_Models[i] = m_Models[m_Models.size() - 1];
			m_Models.pop_back();
			break;
		}
	}
}

void System::AddAndRemoveModels()
{
	for (auto i = 0u; i < m_AddRemoveModelQueue.size(); i++)
	{
		const auto& addRemoveModel = m_AddRemoveModelQueue[i];

		switch (addRemoveModel.operation)
		{
		case AddRemoveModelItem::AddRemoveOperation::ADD:
			AddModelImpl(addRemoveModel.modelToAdd);
			break;

		case AddRemoveModelItem::AddRemoveOperation::REMOVE:
			RemoveModelImpl(addRemoveModel.modelToRemove);
			break;
		}
	}

	m_AddRemoveModelQueue.clear();
}