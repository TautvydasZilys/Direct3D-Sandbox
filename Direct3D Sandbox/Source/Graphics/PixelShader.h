#pragma once
#include "ShaderProgram.h"

class PixelShader :
	public ShaderProgram
{
private:
	ComPtr<ID3D11PixelShader> m_Shader;
	vector<unsigned int> m_TextureOffsets;
	vector<ComPtr<ID3D11SamplerState>> m_SamplerStates;

	void AddTextureOffset(const string& name);
	void AddSamplerState(const string& name);

protected:	
	virtual void ReflectVirtual(ComPtr<ID3D11Device> device, const vector<uint8_t>& shaderBuffer, ComPtr<ID3D11ShaderReflection> shaderReflection, 
		const D3D11_SHADER_DESC& shaderDescription);

public:
	PixelShader(ComPtr<ID3D11Device> device, wstring path);
	virtual ~PixelShader();
	
	virtual void SetRenderParameters(ComPtr<ID3D11DeviceContext> deviceContext, const RenderParameters& renderParameters);
};
