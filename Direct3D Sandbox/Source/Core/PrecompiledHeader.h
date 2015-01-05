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

#define _WIN32_WINNT 0x0602

#include <initguid.h>
#include <unknwn.h>
#include <Windows.h>
#include <wrl.h>

#include <KnownFolders.h>
#include <psapi.h>
#include <ShlObj.h>

#include <d3d11_1.h>
#include <DirectXMath.h>

#include <XAudio2.h>
#include <XAudio2fx.h>
#include <X3DAudio.h>

using namespace Microsoft::WRL;
using namespace std;

#ifdef DrawText
#undef DrawText
#endif

#define ENABLE_FRUSTUM_CULLING 1

#define WIDE2(x) L##x
#define WIDE1(x) WIDE2(x)
#define __WFILE__ WIDE1(__FILE__)