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

	void CreateDesktopWindow();
	void DestroyDesktopWindow();

	void RegisterForRawInput();
	LRESULT HandleRawInput(WPARAM wParam, LPARAM lParam) const;

public:
	DesktopWindowing(int width = 0, int height = 0, bool fullscreen = false);
	~DesktopWindowing();
	
	void DispatchMessages() const;
	LRESULT HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) const;

	inline int GetWidth() const { return m_Width; }
	inline int GetHeight() const { return m_Height; }
	inline bool IsFullscreen() const { return m_Fullscreen; }
	inline HWND GetWindowHandle() const { return m_WindowHandle; }
};

typedef DesktopWindowing Windowing;

#endif