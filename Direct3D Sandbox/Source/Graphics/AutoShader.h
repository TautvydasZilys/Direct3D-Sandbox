#pragma once

#include "IShader.h"
#include "PixelShader.h"
#include "VertexShader.h"

class AutoShader : IShader
{
private:
	VertexShader m_VertexShader;
	PixelShader m_PixelShader;

public:
	AutoShader(ComPtr<ID3D11Device> device, wstring vertexShaderPath, wstring pixelShaderPath);
	virtual ~AutoShader();
	
	virtual ComPtr<ID3D11Buffer> CreateVertexBuffer(ComPtr<ID3D11Device> device, const ModelData& model) const;
	virtual void SetRenderParameters(ComPtr<ID3D11DeviceContext> deviceContext, const RenderParameters& renderParameters);
	virtual const unsigned int* GetInputLayoutSizePtr() const { return m_VertexShader.GetInputLayoutSizePtr(); }
};

