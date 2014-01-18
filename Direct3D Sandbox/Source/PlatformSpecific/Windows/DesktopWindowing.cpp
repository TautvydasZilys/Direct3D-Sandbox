#include "PrecompiledHeader.h"
#include "DesktopWindowing.h"
#include "Tools.h"
#include "Constants.h"

#if !WINDOWS_PHONE

static DesktopWindowing* s_WindowingInstance = nullptr;

DesktopWindowing::DesktopWindowing(int width, int height, bool fullscreen) :
	m_WindowHandle(nullptr), m_ProgramInstance(GetModuleHandle(nullptr)), m_Fullscreen(fullscreen)
{
	if (width == 0 || height == 0)
	{
		width = GetSystemMetrics(SM_CXSCREEN);
		height = GetSystemMetrics(SM_CYSCREEN);
	}
	
	Assert(s_WindowingInstance == nullptr);
	s_WindowingInstance = this;

	CreateDesktopWindow(width, height, fullscreen);
}

DesktopWindowing::~DesktopWindowing()
{
	DestroyDesktopWindow();

	Assert(s_WindowingInstance != nullptr);
	s_WindowingInstance = nullptr;
}

void DesktopWindowing::CreateDesktopWindow(int width, int height, bool fullscreen)
{
	WNDCLASSEX windowInfo;
	DEVMODE screenSettings;
	int posX, posY;
	
	// Setup the windows class with default settings.
	ZeroMemory(&windowInfo, sizeof(WNDCLASSEX));
	windowInfo.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	windowInfo.hInstance = m_ProgramInstance;
	windowInfo.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	windowInfo.hIconSm = windowInfo.hIcon;
	windowInfo.hCursor = LoadCursor(NULL, IDC_ARROW);
	windowInfo.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	windowInfo.lpszMenuName = NULL;
	windowInfo.lpszClassName = Constants::ApplicationName.c_str();
	windowInfo.cbSize = sizeof(WNDCLASSEX);
	
	windowInfo.lpfnWndProc = [](HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT
	{
		return s_WindowingInstance->HandleMessage(hWnd, uMsg, wParam, lParam);
	};
	
	RegisterClassEx(&windowInfo);
	
	if (fullscreen)
	{
		ZeroMemory(&screenSettings, sizeof(screenSettings));
		screenSettings.dmSize = sizeof(DEVMODE);
		screenSettings.dmPelsWidth  = static_cast<DWORD>(width);
		screenSettings.dmPelsHeight = static_cast<DWORD>(height);
		screenSettings.dmBitsPerPel = 32;			
		screenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		ChangeDisplaySettings(&screenSettings, CDS_FULLSCREEN);

		posX = posY = 0;
	}
	else
	{
		// Place the window in the middle of the screen.
		posX = (GetSystemMetrics(SM_CXSCREEN) - width) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - height) / 2;
	}

	m_WindowHandle = CreateWindowEx(WS_EX_APPWINDOW, windowInfo.lpszClassName, windowInfo.lpszClassName, WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP, 
		posX, posY, width, height, NULL, NULL, m_ProgramInstance, NULL);
	Assert(m_WindowHandle != nullptr);

	ShowWindow(m_WindowHandle, SW_SHOW);
	SetForegroundWindow(m_WindowHandle);
	SetFocus(m_WindowHandle);

	ShowCursor(Constants::ShowCursor);
}

void DesktopWindowing::DestroyDesktopWindow()
{
	if (m_Fullscreen)
	{
		ChangeDisplaySettings(nullptr, 0);
	}

	DestroyWindow(m_WindowHandle);
	m_WindowHandle = nullptr;

	UnregisterClass(Constants::ApplicationName.c_str(), m_ProgramInstance);
}

LRESULT DesktopWindowing::HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

void DesktopWindowing::Run()
{
}

#endif