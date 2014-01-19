#pragma once

#include "ConstantBuffer.h"

class ShaderProgram
{
protected:
	vector<ConstantBuffer> m_ConstantBuffers;
	
	ShaderProgram();
	virtual void Reflect(const vector<uint8_t>& shaderBuffer);
	void ReflectConstantBuffers(ComPtr<ID3D11ShaderReflection> shaderReflection, const D3D11_SHADER_DESC& shaderDescription);
public:
	virtual ~ShaderProgram();
};

