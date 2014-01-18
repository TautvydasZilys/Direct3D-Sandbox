#include "PrecompiledHeader.h"

#include "Constants.h"
#include "DesktopWindowing.h"
#include "Input.h"
#include "Tools.h"

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
	RegisterForRawInput();
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

void DesktopWindowing::RegisterForRawInput()
{
	RAWINPUTDEVICE Rid[2];
        
	Rid[0].usUsagePage = 0x01;
	Rid[0].usUsage = 0x02;					// Mouse
	Rid[0].dwFlags = RIDEV_CAPTUREMOUSE;
	Rid[0].hwndTarget = m_WindowHandle;

	Rid[1].usUsagePage = 0x01;
	Rid[1].usUsage = 0x06;					// Keyboard
	Rid[1].dwFlags = 0;
	Rid[1].hwndTarget = m_WindowHandle;

	auto result = RegisterRawInputDevices(Rid, 2, sizeof(Rid[0]));
	Assert(result != 0);
}

void DesktopWindowing::DispatchMessages() const
{
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);			
	}
}

LRESULT DesktopWindowing::HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) const
{	
	switch (uMsg)
	{
		case WM_QUIT:
			Input::GetInstance().Quit();
			return 0;
			
		case WM_INPUT:
			HandleRawInput(lParam, wParam);
			return 0;

		default:
			return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
}

void DesktopWindowing::HandleRawInput(WPARAM wParam, LPARAM lParam) const
{
	unsigned int dataSize;
	auto& input = Input::GetInstance();

	GetRawInputData((HRAWINPUT)lParam, RID_INPUT, NULL, &dataSize, sizeof(RAWINPUTHEADER));
	auto buffer = unique_ptr<unsigned char[]>(new unsigned char[dataSize]);
	GetRawInputData((HRAWINPUT)lParam, RID_INPUT, buffer.get(), &dataSize, sizeof(RAWINPUTHEADER));

	RAWINPUT* raw = (RAWINPUT*)buffer.get();

	if (raw->header.dwType == RIM_TYPEKEYBOARD) 
	{
		if (raw->data.keyboard.Message == WM_KEYDOWN && wParam == 0)
		{
			input.KeyDown(raw->data.keyboard.VKey);
		}
		else if (raw->data.keyboard.Message == WM_KEYUP)
		{
			input.KeyUp(raw->data.keyboard.VKey);
		}
	}
	else if (raw->header.dwType == RIM_TYPEMOUSE && wParam == 0) 
	{
		if (raw->data.mouse.usButtonFlags == RI_MOUSE_WHEEL)
		{
			input.SetMouseWheelDisplacement(static_cast<long>(raw->data.mouse.usButtonData));
		}

		input.SetMouseDisplacement(raw->data.mouse.lLastX, raw->data.mouse.lLastY);
	}
}

#endif	// !WINDOWS_PHONE