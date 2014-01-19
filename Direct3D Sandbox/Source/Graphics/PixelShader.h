#pragma once
#include "ShaderProgram.h"

class PixelShader :
	public ShaderProgram
{
private:
	ComPtr<ID3D11PixelShader> m_Shader;

public:
	PixelShader(ComPtr<ID3D11Device> device, wstring path);
	virtual ~PixelShader();
};

