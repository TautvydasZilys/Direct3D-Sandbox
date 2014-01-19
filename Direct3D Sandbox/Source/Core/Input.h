#pragma once
class Input
{
private:
	static unique_ptr<Input> s_Instance;
	
	bool m_Paused;
	bool m_Quit;
	bool m_KeyMap[256];

	long m_MouseX, m_MouseY;
	long m_PinchDisplacement;
	long m_MouseWheelDisplacement;

	Input();
	Input(const Input&);
	
public:
	~Input();
	static Input& GetInstance();
	
	void KeyDown(int key) { m_KeyMap[key] = true; }
	void KeyUp(int key) { m_KeyMap[key] = false; }
	bool IsKeyDown(int key) const { return m_KeyMap[key]; }

	void SetMouseDisplacement(long x, long y);
	void SetPinchDisplacement(long delta);
	void SetMouseWheelDisplacement(long delta);
	void HandleMouseDisplacement(long& x, long& y);
	long HandlePinchDisplacement();
	long HandleMouseWheelDisplacement();

	void Quit() { m_Quit = true;}
	bool ShouldQuit() const { return m_Quit; }

	void SetPause(bool paused) { m_Paused = paused; }
	bool IsPaused() const { return m_Paused; }
};

