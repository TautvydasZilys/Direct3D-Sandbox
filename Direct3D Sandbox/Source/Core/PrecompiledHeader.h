#pragma once

#ifndef DEBUG
#define	DEBUG _DEBUG
#endif

#if DEBUG
#define _CRTDBG_MAP_ALLOC
#endif

#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <random>
#include <set>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>

#if !WINDOWS_PHONE
#define _WIN32_WINNT 0x0600
#endif

#include <initguid.h>
#include <unknwn.h>
#include <Windows.h>
#include <wrl.h>

#if !WINDOWS_PHONE
#include <psapi.h>
#endif

#include <D3D11.h>
#include <DirectXMath.h>

#if !WINDOWS_PHONE
#include <C:\Program Files (x86)\Microsoft DirectX SDK (June 2010)\Include\XAudio2.h> 
#include <C:\Program Files (x86)\Microsoft DirectX SDK (June 2010)\Include\\XAudio2fx.h>
#include <C:\Program Files (x86)\Microsoft DirectX SDK (June 2010)\Include\X3DAudio.h>
#else
#include <XAudio2.h>
#include <XAudio2fx.h>
#include <X3DAudio.h>
#endif

using namespace Microsoft::WRL;
using namespace std;

#ifdef DrawText
#undef DrawText
#endif

#define ENABLE_FRUSTUM_CULLING 1

#define WIDE2(x) L##x
#define WIDE1(x) WIDE2(x)
#define __WFILE__ WIDE1(__FILE__)

#if WINDOWS_PHONE
extern "C" HRESULT WINAPI D3D11CreateDeviceAndSwapChain(
    _In_opt_ IDXGIAdapter* pAdapter,
    D3D_DRIVER_TYPE DriverType,
    HMODULE Software,
    UINT Flags,
    _In_reads_opt_( FeatureLevels ) CONST D3D_FEATURE_LEVEL* pFeatureLevels,
    UINT FeatureLevels,
    UINT SDKVersion,
    _In_opt_ CONST DXGI_SWAP_CHAIN_DESC* pSwapChainDesc,
    _Out_opt_ IDXGISwapChain** ppSwapChain,
    _Out_opt_ ID3D11Device** ppDevice,
    _Out_opt_ D3D_FEATURE_LEVEL* pFeatureLevel,
    _Out_opt_ ID3D11DeviceContext** ppImmediateContext );
#endif