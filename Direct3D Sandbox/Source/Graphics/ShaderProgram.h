#pragma once

#include "ConstantBuffer.h"

class ShaderProgram
{
protected:
	vector<ConstantBuffer> m_ConstantBuffers;
	vector<ID3D11Buffer*> m_ConstantBufferPtrs;
	vector<unsigned int> m_TextureOffsets;
	
	vector<ID3D11ShaderResourceView*> m_Textures;
	vector<ID3D11SamplerState*> m_SamplerStates;
		
	ShaderProgram();
	virtual void Reflect(const vector<uint8_t>& shaderBuffer, const vector<uint8_t>& metadataBuffer);

	virtual void SetConstantBuffersImpl() const = 0;
	virtual void SetTexturesImpl() = 0;
	virtual void SetSamplersImpl() const = 0;

private:
	void ReflectConstantBuffers(const vector<uint8_t>& metadataBuffer);
	void ReflectOtherResources(const vector<uint8_t>& metadataBuffer);

	void AddTextureOffset(const string& name);
	void AddSamplerState(const string& name);
		
	void SetConstantBuffers() const;
	void SetTextures(const RenderParameters& renderParameters);
	void SetSamplers() const;

public:
	virtual ~ShaderProgram();
	
	virtual void SetRenderParameters(const RenderParameters& renderParameters);
};

