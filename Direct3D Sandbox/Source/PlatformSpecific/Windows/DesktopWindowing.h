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

	void RegisterForRawInput();
	void HandleRawInput(WPARAM wParam, LPARAM lParam) const;

public:
	DesktopWindowing(int width = 0, int height = 0, bool fullscreen = false);
	~DesktopWindowing();
	
	void DispatchMessages() const;
	LRESULT HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) const;

	int GetWidth() const { return m_Width; }
	int GetHeight() const { return m_Height; }
	HWND GetWindowHandle() const { return m_WindowHandle; }
};

#endif