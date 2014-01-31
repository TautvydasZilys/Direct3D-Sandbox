#include "PrecompiledHeader.h"
#include "Constants.h"
#include "Direct3D.h"
#include "Tools.h"

Direct3D::Direct3D(HWND hWnd, int width, int height, bool fullscreen)
{
	ComPtr<IDXGIAdapter1> dxgiAdapter;
	ComPtr<IDXGIOutput> dxgiOutput;

	GetDXGIAdapterAndOutput(dxgiAdapter, dxgiOutput);
	auto refreshRate = GetRefreshRate(dxgiOutput, width, height);
	auto featureLevel = CreateDeviceAndSwapChain(hWnd, width, height, refreshRate, fullscreen);
	CreateBackBufferResources(width, height);
	CreateRasterizerAndBlendStates(width, height);

	PrintAdapterInfo(dxgiAdapter, featureLevel);
}

Direct3D::~Direct3D()
{
}

void Direct3D::GetDXGIAdapterAndOutput(ComPtr<IDXGIAdapter1>& dxgiAdapter, ComPtr<IDXGIOutput>& dxgiOutput) const
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

D3D_FEATURE_LEVEL Direct3D::CreateDeviceAndSwapChain(HWND hWnd, int width, int height, const DXGI_RATIONAL& refreshRate, bool fullscreen)
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
	
	GetSwapChainDescription(hWnd, width, height, refreshRate, fullscreen, swapChainDescription);

	result = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, deviceFlags, 
		featureLevels, sizeof(featureLevels) / sizeof(D3D_FEATURE_LEVEL), D3D11_SDK_VERSION, &swapChainDescription, 
		&m_SwapChain, &m_Device, &supportedFeatureLevel, &m_DeviceContext);
	Assert(result == S_OK);

	return supportedFeatureLevel;
}

void Direct3D::CreateBackBufferResources(int width, int height)
{	
	HRESULT result;
	ComPtr<ID3D11Texture2D> backBuffer;
	D3D11_TEXTURE2D_DESC depthBufferDescription;
	D3D11_DEPTH_STENCIL_DESC depthStencilDescription;
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDescription;

	GetDepthBufferDescription(width, height, depthBufferDescription);
	GetDepthStencilDescription(depthStencilDescription);
	GetDepthStencilViewDescription(depthStencilViewDescription);

	result = m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), &backBuffer);
	Assert(result == S_OK);

	result = m_Device->CreateRenderTargetView(backBuffer.Get(), NULL, &m_RenderTargetView);
	Assert(result == S_OK);

	result = m_Device->CreateTexture2D(&depthBufferDescription, nullptr, &m_DepthStencilBuffer);
	Assert(result == S_OK);

	depthStencilDescription.DepthEnable = true;
	result = m_Device->CreateDepthStencilState(&depthStencilDescription, &m_DepthStencilState);
	Assert(result == S_OK);

	depthStencilDescription.DepthEnable = false;
	result = m_Device->CreateDepthStencilState(&depthStencilDescription, &m_DisabledDepthStencilState);
	Assert(result == S_OK);

	result = m_Device->CreateDepthStencilView(m_DepthStencilBuffer.Get(), &depthStencilViewDescription, &m_DepthStencilView);
	Assert(result == S_OK);
}

void Direct3D::CreateRasterizerAndBlendStates(int width, int height)
{
	HRESULT result;
	D3D11_RASTERIZER_DESC rasterizerDescription;
	D3D11_BLEND_DESC blendDescription;
	D3D11_VIEWPORT viewport;
	float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

	GetRasterizerStateDescription(rasterizerDescription);
	GetViewPort(width, height, viewport);
	GetBlendStateDescription(blendDescription);

	result = m_Device->CreateRasterizerState(&rasterizerDescription, &m_RasterizerState);
	Assert(result == S_OK);

	result = m_Device->CreateBlendState(&blendDescription, &m_BlendState);
	Assert(result == S_OK);

	m_DeviceContext->RSSetState(m_RasterizerState.Get());
	m_DeviceContext->RSSetViewports(1, &viewport);
	m_DeviceContext->OMSetBlendState(m_BlendState.Get(), blendFactor, 0xFFFFFFFF);
}

void Direct3D::PrintAdapterInfo(ComPtr<IDXGIAdapter1> dxgiAdapter, D3D_FEATURE_LEVEL featureLevel) const
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

DXGI_RATIONAL Direct3D::GetRefreshRate(ComPtr<IDXGIOutput> dxgiOutput, int width, int height) const
{
	DXGI_RATIONAL refreshRate;

	refreshRate.Numerator = 0;
	refreshRate.Denominator = 1;

	if (Constants::VSyncEnabled)
	{
		UINT numberOfDisplayModes = 0;

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

void Direct3D::GetSwapChainDescription(HWND hWnd, int width, int height, const DXGI_RATIONAL& refreshRate,
										bool fullscreen, DXGI_SWAP_CHAIN_DESC& swapChainDescription) const
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
	swapChainDescription.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDescription.OutputWindow = hWnd;
	swapChainDescription.Windowed = !fullscreen;

	swapChainDescription.SampleDesc.Count = Constants::MultiSampingAntiAliasing;
	swapChainDescription.SampleDesc.Quality = 0;
}

void Direct3D::GetDepthBufferDescription(int width, int height, D3D11_TEXTURE2D_DESC& depthBufferDescription) const
{
	ZeroMemory(&depthBufferDescription, sizeof(depthBufferDescription));

	depthBufferDescription.Width = width;
	depthBufferDescription.Height = height;

	depthBufferDescription.MipLevels = 1;
	depthBufferDescription.ArraySize = 1;

	depthBufferDescription.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDescription.SampleDesc.Count = 1;
	depthBufferDescription.SampleDesc.Quality = 0;

	depthBufferDescription.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDescription.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDescription.CPUAccessFlags = 0;
	depthBufferDescription.MiscFlags = 0;
}

void Direct3D::GetDepthStencilDescription(D3D11_DEPTH_STENCIL_DESC& depthStencilDescription) const
{
	ZeroMemory(&depthStencilDescription, sizeof(depthStencilDescription));

	depthStencilDescription.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDescription.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDescription.StencilEnable = false;
	depthStencilDescription.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
	depthStencilDescription.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;

	depthStencilDescription.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDescription.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDescription.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDescription.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	depthStencilDescription.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDescription.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDescription.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDescription.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
}

void Direct3D::GetDepthStencilViewDescription(D3D11_DEPTH_STENCIL_VIEW_DESC& depthStencilViewDescription) const
{
	ZeroMemory(&depthStencilViewDescription, sizeof(depthStencilViewDescription));

	depthStencilViewDescription.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDescription.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDescription.Flags = 0;
	depthStencilViewDescription.Texture2D.MipSlice = 0;
}

void Direct3D::GetRasterizerStateDescription(D3D11_RASTERIZER_DESC& rasterizerDescription) const
{
	ZeroMemory(&rasterizerDescription, sizeof(rasterizerDescription));

	rasterizerDescription.FillMode = Constants::D3DFillMode;
	rasterizerDescription.CullMode = D3D11_CULL_BACK;
	rasterizerDescription.FrontCounterClockwise = false;

	rasterizerDescription.DepthBias = 0;
	rasterizerDescription.DepthBiasClamp = 0.0f;
	rasterizerDescription.SlopeScaledDepthBias = 0.0f;
	
	rasterizerDescription.DepthClipEnable = true;
	rasterizerDescription.ScissorEnable = false;
	rasterizerDescription.MultisampleEnable = true;
	rasterizerDescription.AntialiasedLineEnable = false;
}

void Direct3D::GetViewPort(int width, int height, D3D11_VIEWPORT& viewport) const
{
	ZeroMemory(&viewport, sizeof(viewport));

	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;

	viewport.Width = static_cast<float>(width);
	viewport.Height = static_cast<float>(height);

	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
}

void Direct3D::GetBlendStateDescription(D3D11_BLEND_DESC& blendDescription) const
{
	ZeroMemory(&blendDescription, sizeof(blendDescription));

	blendDescription.AlphaToCoverageEnable = false;
	blendDescription.IndependentBlendEnable = false;

	blendDescription.RenderTarget[0].BlendEnable = true;
	blendDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDescription.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendDescription.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDescription.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDescription.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDescription.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDescription.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
}

void Direct3D::StartDrawing(float red, float green, float blue, float alpha)
{
	float color[] = { red, green, blue, alpha };

	m_DeviceContext->ClearRenderTargetView(m_RenderTargetView.Get(), color);
	m_DeviceContext->ClearDepthStencilView(m_DepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void Direct3D::SwapBuffers()
{
	if (Constants::VSyncEnabled)
	{
		Assert(m_SwapChain->Present(1, 0) == S_OK);
	}
	else
	{
		Assert(m_SwapChain->Present(0, 0) == S_OK);
	}
}

void Direct3D::SetBackBufferAsRenderTarget()
{
	m_DeviceContext->OMSetRenderTargets(1, m_RenderTargetView.GetAddressOf(), m_DepthStencilView.Get());
}

void Direct3D::TurnZBufferOn()
{
	m_DeviceContext->OMSetDepthStencilState(m_DepthStencilState.Get(), 1);
}

void Direct3D::TurnZBufferOff()
{
	m_DeviceContext->OMSetDepthStencilState(m_DisabledDepthStencilState.Get(), 1);
}