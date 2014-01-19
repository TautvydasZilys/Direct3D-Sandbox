#pragma once

class Direct3D
{
private:	
	ComPtr<ID3D11Device> m_Device;
	ComPtr<ID3D11DeviceContext> m_DeviceContext;
	ComPtr<IDXGISwapChain> m_SwapChain;
	ComPtr<ID3D11RenderTargetView> m_RenderTargetView;
	ComPtr<ID3D11Texture2D> m_DepthStencilBuffer;
	ComPtr<ID3D11DepthStencilState> m_DepthStencilState;
	ComPtr<ID3D11DepthStencilState> m_DisabledDepthStencilState;
	ComPtr<ID3D11DepthStencilView> m_DepthStencilView;
	ComPtr<ID3D11RasterizerState> m_RasterState;
	ComPtr<ID3D11BlendState> m_AlphaBlendingState;
	D3D11_VIEWPORT m_Viewport;

	void Direct3D::GetDXGIAdapterAndOutput(ComPtr<IDXGIAdapter1>& dxgiAdapter, ComPtr<IDXGIOutput>& dxgiOutput);
	DXGI_RATIONAL GetRefreshRate(ComPtr<IDXGIOutput> dxgiOutput, int width, int height);
	void PrintAdapterInfo(ComPtr<IDXGIAdapter1> dxgiAdapter, D3D_FEATURE_LEVEL featureLevel);
	void GetSwapChainDescription(HWND hWnd, int width, int height, const DXGI_RATIONAL& refreshRate, bool windowed, DXGI_SWAP_CHAIN_DESC& swapChainDescription);
	D3D_FEATURE_LEVEL CreateDeviceAndSwapChain(HWND hWnd, int width, int height, const DXGI_RATIONAL& refreshRate, bool windowed);

public:
	Direct3D(HWND hWnd, int width, int height, bool windowed);
	~Direct3D();
};

