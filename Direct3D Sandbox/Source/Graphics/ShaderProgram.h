#pragma once

#include "ConstantBuffer.h"

class ShaderProgram
{
protected:
	vector<ConstantBuffer> m_ConstantBuffers;
	vector<ID3D11Buffer*> m_ConstantBufferPtrs;
	vector<unsigned int> m_TextureOffsets;
	vector<ID3D11SamplerState*> m_SamplerStates;
		
	ShaderProgram();
	virtual void Reflect(ComPtr<ID3D11Device> device, const vector<uint8_t>& shaderBuffer, const vector<uint8_t>& metadataBuffer);

private:
	void ReflectConstantBuffers(ComPtr<ID3D11Device> device, const vector<uint8_t>& metadataBuffer);
	void ReflectOtherResources(const vector<uint8_t>& metadataBuffer);

	void AddTextureOffset(const string& name);
	void AddSamplerState(const string& name);

public:
	virtual ~ShaderProgram();
	
	virtual void SetRenderParameters(ComPtr<ID3D11DeviceContext> deviceContext, const RenderParameters& renderParameters);
};

