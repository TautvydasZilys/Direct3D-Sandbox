#pragma once

#include "ConstantBufferField.h"

struct RenderParameters;

class ConstantBuffer
{
private:
	ComPtr<ID3D11Buffer> m_Buffer;
	vector<ConstantBufferField> m_Fields;
	int m_Size;
	
	void SetRenderParametersNoCheck(ComPtr<ID3D11DeviceContext> deviceContext);

	ConstantBuffer(const ConstantBuffer& other);

public:
	ConstantBuffer(ComPtr<ID3D11Device> device, const vector<uint8_t>& metadataBuffer, unsigned int& byteOffset);
	ConstantBuffer(ConstantBuffer&& other);

	~ConstantBuffer();

	void SetRenderParameters(ComPtr<ID3D11DeviceContext> deviceContext, const RenderParameters& renderParameters);
	ID3D11Buffer* GetPtr() const { return m_Buffer.Get(); }
};