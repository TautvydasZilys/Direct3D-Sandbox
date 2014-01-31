#pragma once

#include "ConstantBuffer.h"

class ShaderProgram
{
protected:
	vector<ConstantBuffer> m_ConstantBuffers;
	vector<ID3D11Buffer*> m_ConstantBufferPtrs;
	
	ShaderProgram();
	virtual void Reflect(ComPtr<ID3D11Device> device, const vector<uint8_t>& metadataBuffer);

	void ReflectConstantBuffers(ComPtr<ID3D11Device> device, const vector<uint8_t>& metadataBuffer);

public:
	virtual ~ShaderProgram();
	
	virtual void SetRenderParameters(ComPtr<ID3D11DeviceContext> deviceContext, const RenderParameters& renderParameters);
};

