#pragma once
#include "ShaderProgram.h"

class PixelShader :
	public ShaderProgram
{
private:
	ComPtr<ID3D11PixelShader> m_Shader;
	
	virtual void SetConstantBuffersImpl() const;
	virtual void SetTexturesImpl();
	virtual void SetSamplersImpl() const;
	
public:
	PixelShader(wstring path);
	virtual ~PixelShader();
	
	virtual void SetRenderParameters(const RenderParameters& renderParameters);
};
