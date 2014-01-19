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

	/*ComPtr<ID3D11VertexShader> m_VertexShader;
	ComPtr<ID3D11PixelShader> m_PixelShader;

	vector<ConstantBuffer> m_VertexShaderConstantBuffers;
	vector<ConstantBuffer> m_PixelShaderConstantBuffers;*/

	static vector<ConstantBuffer> ReflectConstantBuffers(ComPtr<ID3D11ShaderReflection> shaderReflection, const D3D11_SHADER_DESC& shaderDescription);

public:
	AutoShader(ComPtr<ID3D11Device> device, wstring vertexShaderPath, wstring pixelShaderPath);
	virtual ~AutoShader();

	virtual void Render(const GlobalParameters& globalParameters, const ModelParameters& modelParameters);
};

