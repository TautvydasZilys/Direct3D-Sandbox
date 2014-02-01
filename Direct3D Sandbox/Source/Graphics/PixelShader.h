#pragma once
#include "ShaderProgram.h"

class PixelShader :
	public ShaderProgram
{
private:
	ComPtr<ID3D11PixelShader> m_Shader;
	
	virtual void SetConstantBuffersImpl(ComPtr<ID3D11DeviceContext> deviceContext) const;
	virtual void SetTexturesImpl(ComPtr<ID3D11DeviceContext> deviceContext, const vector<ID3D11ShaderResourceView*> textures) const;
	virtual void SetSamplersImpl(ComPtr<ID3D11DeviceContext> deviceContext) const;
	
public:
	PixelShader(ComPtr<ID3D11Device> device, wstring path);
	virtual ~PixelShader();
	
	virtual void SetRenderParameters(ComPtr<ID3D11DeviceContext> deviceContext, const RenderParameters& renderParameters);
};
