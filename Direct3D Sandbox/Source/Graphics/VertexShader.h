#pragma once

#include "ShaderProgram.h"

class VertexShader :
	public ShaderProgram
{
private:
	ComPtr<ID3D11VertexShader> m_Shader;
	
	virtual void ReflectVirtual(ComPtr<ID3D11Device> device, ComPtr<ID3D11ShaderReflection> shaderReflection, const D3D11_SHADER_DESC& shaderDescription);
	void ReflectInputLayout(ComPtr<ID3D11Device> device, ComPtr<ID3D11ShaderReflection> shaderReflection, const D3D11_SHADER_DESC& shaderDescription);

public:
	VertexShader(ComPtr<ID3D11Device> device, wstring path);
	virtual ~VertexShader();
};

