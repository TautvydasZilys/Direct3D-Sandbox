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
	
	virtual ComPtr<ID3D11Buffer> CreateVertexBuffer(const ModelData& model) const;
	virtual void SetRenderParameters(const RenderParameters& renderParameters);
	virtual const unsigned int* GetInputLayoutSizePtr() const { return m_VertexShader.GetInputLayoutSizePtr(); }
};

