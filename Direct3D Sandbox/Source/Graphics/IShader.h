#pragma once

struct RenderParameters;
struct VertexParameters;

class IShader
{
protected:
	IShader();
	IShader(IShader& other);
	IShader& operator=(const IShader& other);

public:
	virtual ~IShader();
	
	virtual ComPtr<ID3D11Buffer> CreateVertexBuffer(ComPtr<ID3D11Device> device, const vector<VertexParameters>& vertices) const = 0;
	virtual void SetRenderParameters(ComPtr<ID3D11DeviceContext> deviceContext, const RenderParameters& renderParameters) = 0;
};

