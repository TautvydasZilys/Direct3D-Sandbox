#pragma once
#include "ShaderProgram.h"

class PixelShader :
	public ShaderProgram
{
private:
	ComPtr<ID3D11PixelShader> m_Shader;
	vector<unsigned int> m_TextureOffsets;
	vector<ID3D11SamplerState*> m_SamplerStates;

	void AddTextureOffset(const string& name);
	void AddSamplerState(const string& name);

	void SetConstantBuffers(ComPtr<ID3D11DeviceContext> deviceContext) const;
	void SetTextures(ComPtr<ID3D11DeviceContext> deviceContext, const RenderParameters& renderParameters) const;
	void SetSamplers(ComPtr<ID3D11DeviceContext> deviceContext) const;

protected:
	virtual void Reflect(ComPtr<ID3D11Device> device, const vector<uint8_t>& metadataBuffer);

public:
	PixelShader(ComPtr<ID3D11Device> device, wstring path);
	virtual ~PixelShader();
	
	virtual void SetRenderParameters(ComPtr<ID3D11DeviceContext> deviceContext, const RenderParameters& renderParameters);
};
