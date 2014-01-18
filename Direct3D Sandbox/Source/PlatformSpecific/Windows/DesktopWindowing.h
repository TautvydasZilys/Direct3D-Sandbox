#pragma once

#if !WINDOWS_PHONE

class DesktopWindowing
{
private:
	HWND m_WindowHandle;
	HMODULE m_ProgramInstance;

	int m_Width;
	int m_Height;
	bool m_Fullscreen;

	void CreateDesktopWindow(int width, int height, bool fullscreen);
	void DestroyDesktopWindow();

public:
	DesktopWindowing(int width = 0, int height = 0, bool fullscreen = false);
	~DesktopWindowing();
	
	void DispatchMessages();
	LRESULT HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);	

	int GetWidth() { return m_Width; }
	int GetHeight() { return m_Height; }
	HWND GetWindowHandle() { return m_WindowHandle; }
};

#endif