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
	ComPtr<ID3D11RasterizerState> m_RasterizerState;
	ComPtr<ID3D11BlendState> m_BlendState;
	D3D11_VIEWPORT m_Viewport;

	void Direct3D::GetDXGIAdapterAndOutput(ComPtr<IDXGIAdapter1>& dxgiAdapter, ComPtr<IDXGIOutput>& dxgiOutput) const;
	D3D_FEATURE_LEVEL CreateDeviceAndSwapChain(HWND hWnd, int width, int height, const DXGI_RATIONAL& refreshRate, bool fullscreen);
	void CreateBackBufferResources(int width, int height);
	void CreateRasterizerAndBlendStates(int width, int height);

	void PrintAdapterInfo(ComPtr<IDXGIAdapter1> dxgiAdapter, D3D_FEATURE_LEVEL featureLevel) const;
	
	DXGI_RATIONAL GetRefreshRate(ComPtr<IDXGIOutput> dxgiOutput, int width, int height) const;
	void GetSwapChainDescription(HWND hWnd, int width, int height, const DXGI_RATIONAL& refreshRate, bool fullscreen, DXGI_SWAP_CHAIN_DESC& swapChainDescription) const;
	void GetDepthBufferDescription(int width, int height, D3D11_TEXTURE2D_DESC& depthBufferDescription) const;
	void GetDepthStencilDescription(D3D11_DEPTH_STENCIL_DESC& depthStencilDescription) const;
	void GetDepthStencilViewDescription(D3D11_DEPTH_STENCIL_VIEW_DESC& depthStencilViewDescription) const;
	void GetRasterizerStateDescription(D3D11_RASTERIZER_DESC& rasterizerDescription) const;
	void GetViewPort(int width, int height, D3D11_VIEWPORT& viewport) const;
	void GetBlendStateDescription(D3D11_BLEND_DESC& blendDescription) const;

public:
	Direct3D(HWND hWnd, int width, int height, bool fullscreen);
	~Direct3D();

	ComPtr<ID3D11Device> GetDevice() const { return m_Device; }
	ComPtr<ID3D11DeviceContext> GetDeviceContext() const { return m_DeviceContext; }
	
	void StartDrawing(float red = 0.0f, float green = 0.0f, float blue = 0.0f, float alpha = 1.0f);
	void SwapBuffers();

	void SetBackBufferAsRenderTarget();
	
	void TurnZBufferOn();
	void TurnZBufferOff();
};

