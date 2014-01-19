#pragma once

#include "ShaderProgram.h"

class VertexShader :
	public ShaderProgram
{
private:
	ComPtr<ID3D11VertexShader> m_Shader;

public:
	VertexShader(ComPtr<ID3D11Device> device, wstring path);
	virtual ~VertexShader();
};

