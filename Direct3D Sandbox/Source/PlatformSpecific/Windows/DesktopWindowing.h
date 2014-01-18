#pragma once

#if !WINDOWS_PHONE

class DesktopWindowing
{
private:
	HWND m_WindowHandle;
	HMODULE m_ProgramInstance;

	bool m_Fullscreen;

	void CreateDesktopWindow(int width, int height, bool fullscreen);
	void DestroyDesktopWindow();

public:
	DesktopWindowing(int width = 0, int height = 0, bool fullscreen = false);
	~DesktopWindowing();

	LRESULT HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	void Run();
};

#endif