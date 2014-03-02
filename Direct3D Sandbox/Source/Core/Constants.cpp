#include "PrecompiledHeader.h"
#include "Constants.h"

const wstring Constants::ApplicationName = L"Direct3D Sandbox";
const bool Constants::ShowCursor = false;

#if !WINDOWS_PHONE
const bool Constants::VSyncEnabled = false;
const int Constants::MultiSampingAntiAliasing = 4;
#else
const bool Constants::VSyncEnabled = true;
const int Constants::MultiSampingAntiAliasing = 1;
#endif

const UINT Constants::D3DDeviceFlags = D3D11_CREATE_DEVICE_SINGLETHREADED;

const D3D11_FILL_MODE Constants::D3DFillMode = D3D11_FILL_SOLID;

const float Constants::CameraNear = 0.01f;
const float Constants::CameraFar = 10000.0f;
const float Constants::VerticalFieldOfView = 95.0f;

#if WINDOWS_PHONE
const float Constants::DefaultMouseSensitivity = 0.2f;
#else
const float Constants::DefaultMouseSensitivity = 0.5f;
#endif

const float Constants::GravityConstant = -9.81f;