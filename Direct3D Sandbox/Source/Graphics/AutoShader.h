#pragma once

#include "IShader.h"
#include "PixelShader.h"
#include "VertexShader.h"

class AutoShader : public IShader
{
private:
	VertexShader m_VertexShader;
	PixelShader m_PixelShader;

public:
	AutoShader(wstring vertexShaderPath, wstring pixelShaderPath);
	virtual ~AutoShader();
	
	virtual ComPtr<ID3D11Buffer> CreateVertexBuffer(unsigned int vertexCount, unsigned int semanticIndex, D3D11_USAGE usage) const;
	virtual ComPtr<ID3D11Buffer> CreateVertexBuffer(unsigned int vertexCount, const VertexParameters vertices[], unsigned int semanticIndex, 
		D3D11_USAGE usage = D3D11_USAGE::D3D11_USAGE_IMMUTABLE) const;

	virtual void UploadVertexData(ID3D11Buffer* vertexBuffer, unsigned int vertexCount, const VertexParameters vertices[], unsigned int semanticIndex) const;

	virtual void SetRenderParameters(const RenderParameters& renderParameters);
	virtual const unsigned int* GetInputLayoutStrides() const { return m_VertexShader.GetInputLayoutStrides(); }
	virtual const unsigned int* GetInputLayoutOffsets() const { return m_VertexShader.GetInputLayoutOffsets(); }
};