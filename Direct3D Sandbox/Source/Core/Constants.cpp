#include "PrecompiledHeader.h"
#include "Constants.h"

const wstring Constants::ApplicationName = L"Stereo 3D Player";
const bool Constants::ShowCursor = false;

const bool Constants::VSyncEnabled = true;
const int Constants::MultiSampingAntiAliasing = 1;

const UINT Constants::D3DDeviceFlags = D3D11_CREATE_DEVICE_SINGLETHREADED;

const D3D11_FILL_MODE Constants::D3DFillMode = D3D11_FILL_SOLID;

const float Constants::CameraNear = 0.01f;
const float Constants::CameraFar = 10000.0f;
const float Constants::VerticalFieldOfView = 95.0f;

const float Constants::DefaultMouseSensitivity = 0.5f;