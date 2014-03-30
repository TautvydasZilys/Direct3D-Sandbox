#pragma once

#include "DirectionalLight.h"
#include "Input.h"
#include "Source\Graphics\Direct3D.h"
#include "Source\Models\IModelInstance.h"
#include "Source\PlatformSpecific\Windows\DesktopWindowing.h"
#include "Source\PlatformSpecific\WindowsPhone\PhoneWindowing.h"

class Camera;
class Font;

class System
{
private:
	static System* s_Instance;
	Windowing m_Windowing;

	Direct3D m_Direct3D;
	Input& m_Input;
	float m_MouseSensitivity;

	float m_FrameTime;
	double m_CurrentTime;
	double m_LastFpsTime;
	int m_Fps;
	
	unique_ptr<Camera> m_Camera;
	unique_ptr<Camera> m_OrthoCamera;
	DirectionalLight m_Light;
	vector<shared_ptr<IModelInstance>> m_Models;
	
	Font* m_Font;

	void Update();
	void Draw();
	void IncrementFpsCounter();

	void UpdateInput();

public:
	System();
	~System();

	void Run();
	inline System& GetInstance() { return *s_Instance; }

	void AddModel(shared_ptr<IModelInstance> model);
	void RemoveModel(const IModelInstance* model);
};

