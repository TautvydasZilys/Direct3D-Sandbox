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
	static LRESULT HandleRawInput(WPARAM wParam, LPARAM lParam);

public:
	DesktopWindowing(int width = 0, int height = 0, bool fullscreen = false);
	~DesktopWindowing();
	
	void DispatchMessages() const;
	static LRESULT HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	inline int GetWidth() const { return m_Width; }
	inline int GetHeight() const { return m_Height; }
	inline float GetAspectRatio() const { return static_cast<float>(m_Width) / static_cast<float>(m_Height); }
	inline bool IsFullscreen() const { return m_Fullscreen; }
	inline HWND GetWindowHandle() const { return m_WindowHandle; }
};

typedef DesktopWindowing Windowing;

#endif