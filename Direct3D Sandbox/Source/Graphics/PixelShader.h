#pragma once
#include "ShaderProgram.h"

class PixelShader :
	public ShaderProgram
{
private:
	ComPtr<ID3D11PixelShader> m_Shader;

	void SetConstantBuffers(ComPtr<ID3D11DeviceContext> deviceContext) const;
	void SetTextures(ComPtr<ID3D11DeviceContext> deviceContext, const RenderParameters& renderParameters) const;
	void SetSamplers(ComPtr<ID3D11DeviceContext> deviceContext) const;
	
public:
	PixelShader(ComPtr<ID3D11Device> device, wstring path);
	virtual ~PixelShader();
	
	virtual void SetRenderParameters(ComPtr<ID3D11DeviceContext> deviceContext, const RenderParameters& renderParameters);
};
