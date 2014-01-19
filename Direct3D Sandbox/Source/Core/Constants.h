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
};

