#pragma once

struct ModelData;
struct RenderParameters;

class IShader
{
protected:
	IShader();
	IShader(IShader& other);
	IShader& operator=(const IShader& other);

public:
	virtual ~IShader();
	
	virtual ComPtr<ID3D11Buffer> CreateVertexBuffer(ComPtr<ID3D11Device> device, const ModelData& model) const = 0;
	virtual void SetRenderParameters(ComPtr<ID3D11DeviceContext> deviceContext, const RenderParameters& renderParameters) = 0;
	virtual const unsigned int* GetInputLayoutSizePtr() const = 0;
};

