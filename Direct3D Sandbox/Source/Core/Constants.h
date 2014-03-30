#pragma once

class Constants
{
private:
	Constants();
	~Constants();

public:
	static const wstring ApplicationName;
	static const bool ShowCursor;
	static const bool VSyncEnabled;

	static const int MultiSampingAntiAliasing;
	static const UINT D3DDeviceFlags;

	static const D3D11_FILL_MODE D3DFillMode;

	static const float CameraNear;
	static const float CameraFar;
	static const float VerticalFieldOfView;

	static const float DefaultMouseSensitivity;

	static const float GravityConstant;
	
	static const int StartingZombieCount;
	static const int MaxZombies;
	static const float SpawnIntervalInSeconds;
};

