#pragma once

#include "ConstantBuffer.h"
#include "IShader.h"
#include "PixelShader.h"
#include "VertexShader.h"

class AutoShader : IShader
{
private:
	VertexShader vertexShader;
	PixelShader pixelShader;

public:
	AutoShader(ComPtr<ID3D11Device> device, wstring vertexShaderPath, wstring pixelShaderPath);
	virtual ~AutoShader();

	virtual void Render(const GlobalParameters& globalParameters, const ModelParameters& modelParameters);
};

