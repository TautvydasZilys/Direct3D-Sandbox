#include "PrecompiledHeader.h"
#include "Constants.h"
#include "Direct3D.h"
#include "Tools.h"

Direct3D::Direct3D(HWND hWnd, int width, int height, bool windowed)
{
	ComPtr<IDXGIAdapter1> dxgiAdapter;
	ComPtr<IDXGIOutput> dxgiOutput;

	GetDXGIAdapterAndOutput(dxgiAdapter, dxgiOutput);
	auto refreshRate = GetRefreshRate(dxgiOutput, width, height);
	auto featureLevel = CreateDeviceAndSwapChain(hWnd, width, height, refreshRate, windowed);

	PrintAdapterInfo(dxgiAdapter, featureLevel);
}

Direct3D::~Direct3D()
{
}

void Direct3D::GetDXGIAdapterAndOutput(ComPtr<IDXGIAdapter1>& dxgiAdapter, ComPtr<IDXGIOutput>& dxgiOutput)
{
	HRESULT result;
	ComPtr<IDXGIFactory1> dxgiFactory;

	result = CreateDXGIFactory1(__uuidof(IDXGIFactory1), &dxgiFactory);
	Assert(result == S_OK);

	result = dxgiFactory->EnumAdapters1(0, &dxgiAdapter);
	Assert(result == S_OK);

	result = dxgiAdapter->EnumOutputs(0, &dxgiOutput);
	Assert(result == S_OK);
}

void Direct3D::PrintAdapterInfo(ComPtr<IDXGIAdapter1> dxgiAdapter, D3D_FEATURE_LEVEL featureLevel)
{
	HRESULT result;
	DXGI_ADAPTER_DESC1 adapterDescription;
	wstringstream adapterInfoReport;

	result = dxgiAdapter->GetDesc1(&adapterDescription);
	Assert(result == S_OK);

	adapterInfoReport << "--------------------- Adapter information ---------------------" << endl;
	adapterInfoReport << "\tDescription: \t\t\t" << adapterDescription.Description << endl;
	adapterInfoReport << "\tDedicated memory: \t\t" << adapterDescription.DedicatedVideoMemory / (1024 * 1024) << " MB" << endl;
	adapterInfoReport << "\tShared system memory: \t" << adapterDescription.SharedSystemMemory / (1024 * 1024) << " MB" << endl;
	adapterInfoReport << "\tDirect3D feature level:\t" << (featureLevel >> 12) << "." << ((featureLevel >> 8) & 0xF) << endl;
	adapterInfoReport << "---------------------------------------------------------------" << endl;

	OutputDebugStringW(adapterInfoReport.str().c_str());
}

DXGI_RATIONAL Direct3D::GetRefreshRate(ComPtr<IDXGIOutput> dxgiOutput, int width, int height)
{
	DXGI_RATIONAL refreshRate;

	refreshRate.Numerator = 0;
	refreshRate.Denominator = 1;


	if (Constants::VSyncEnabled)
	{
		UINT numberOfDisplayModes;

		auto result = dxgiOutput->GetDisplayModeList(DXGI_FORMAT_B8G8R8A8_UNORM, 0, &numberOfDisplayModes, nullptr);
		if (result == S_OK)
		{
			unique_ptr<DXGI_MODE_DESC[]> displayModes(new DXGI_MODE_DESC[numberOfDisplayModes]);
		
			result = dxgiOutput->GetDisplayModeList(DXGI_FORMAT_B8G8R8A8_UNORM, 0, &numberOfDisplayModes, displayModes.get());
			Assert(result == S_OK);

			float maxRatio = 0.0f;
			for (auto i = 0u; i < numberOfDisplayModes; i++)
			{
				auto& mode = displayModes[i];
				auto ratio = static_cast<float>(mode.RefreshRate.Numerator) / static_cast<float>(mode.RefreshRate.Denominator);

				if (mode.Width == width && mode.Height == height && maxRatio < ratio)
				{
					refreshRate.Numerator = mode.RefreshRate.Numerator;
					refreshRate.Denominator = mode.RefreshRate.Denominator;
				
					maxRatio = static_cast<float>(refreshRate.Numerator) / static_cast<float>(refreshRate.Denominator);
				}
			}
		}
	}

	return refreshRate;
}

void Direct3D::GetSwapChainDescription(HWND hWnd, int width, int height, const DXGI_RATIONAL& refreshRate, bool windowed, DXGI_SWAP_CHAIN_DESC& swapChainDescription)
{
	ZeroMemory(&swapChainDescription, sizeof(swapChainDescription));

	swapChainDescription.BufferDesc.Width = width;
	swapChainDescription.BufferDesc.Height = height;

	swapChainDescription.BufferDesc.RefreshRate.Numerator = refreshRate.Numerator;
	swapChainDescription.BufferDesc.RefreshRate.Denominator = refreshRate.Denominator;

	swapChainDescription.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;

	swapChainDescription.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_PROGRESSIVE;
	swapChainDescription.BufferDesc.Scaling = DXGI_MODE_SCALING_CENTERED;

	swapChainDescription.BufferCount = 1;

	swapChainDescription.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	swapChainDescription.OutputWindow = hWnd;

	swapChainDescription.SampleDesc.Count = Constants::MultiSampingAntiAliasing;
	swapChainDescription.SampleDesc.Quality = 0;

	swapChainDescription.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	swapChainDescription.Windowed = windowed;
}

D3D_FEATURE_LEVEL Direct3D::CreateDeviceAndSwapChain(HWND hWnd, int width, int height, const DXGI_RATIONAL& refreshRate, bool windowed)
{
	HRESULT result;
	DXGI_SWAP_CHAIN_DESC swapChainDescription;
	UINT deviceFlags = Constants::D3DDeviceFlags;
	D3D_FEATURE_LEVEL supportedFeatureLevel;
	D3D_FEATURE_LEVEL featureLevels[] = 
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1,
	};

#if DEBUG
	deviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	
	GetSwapChainDescription(hWnd, width, height, refreshRate, windowed, swapChainDescription);

	result = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, deviceFlags, 
		featureLevels, sizeof(featureLevels) / sizeof(D3D_FEATURE_LEVEL), D3D11_SDK_VERSION, &swapChainDescription, 
		&m_SwapChain, &m_Device, &supportedFeatureLevel, &m_DeviceContext);
	Assert(result == S_OK);

	return supportedFeatureLevel;
}