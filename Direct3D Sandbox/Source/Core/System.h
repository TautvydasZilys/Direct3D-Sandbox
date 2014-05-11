#pragma once

#include "DirectionalLight.h"
#include "Input.h"
#include "Source\Graphics\Direct3D.h"
#include "Source\Models\IModelInstance.h"
#include "Source\PlatformSpecific\Windows\DesktopWindowing.h"
#include "Source\PlatformSpecific\WindowsPhone\PhoneWindowing.h"

class Camera;

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
	int m_LastFrameFps;
	
	unique_ptr<Camera> m_Camera;
	unique_ptr<Camera> m_OrthoCamera;
	DirectionalLight m_Light;
	
	void Update();
	void Draw();
	void IncrementFpsCounter();

	void UpdateInput();

	struct AddRemoveModelItem
	{
		enum class AddRemoveOperation
		{
			ADD,
			REMOVE
		};

		AddRemoveOperation operation;
		shared_ptr<IModelInstance> modelToAdd;
		const IModelInstance* modelToRemove;
	};

	vector<AddRemoveModelItem> m_AddRemoveModelQueue;
	vector<shared_ptr<IModelInstance>> m_Models;

	void AddModelImpl(shared_ptr<IModelInstance> model);
	void RemoveModelImpl(const IModelInstance* model);
	void AddAndRemoveModels();

	System(const System& other);

public:
	System();
	~System();

	void Run();
	inline static System& GetInstance() { return *s_Instance; }
	inline float GetMouseSensitivity() const { return m_MouseSensitivity; }

	void AddModel(shared_ptr<IModelInstance> model);
	void RemoveModel(const IModelInstance* model);
};

