#include "PrecompiledHeader.h"
#include "Constants.h"

const wstring Constants::ApplicationName = L"Direct3D Sandbox";
const bool Constants::ShowCursor = false;

#if !WINDOWS_PHONE
const bool Constants::VSyncEnabled = false;
#else
const bool Constants::VSyncEnabled = true;
#endif

const int Constants::MultiSampingAntiAliasing = 1;
const UINT Constants::D3DDeviceFlags = D3D11_CREATE_DEVICE_SINGLETHREADED;

const D3D11_FILL_MODE Constants::D3DFillMode = D3D11_FILL_SOLID;