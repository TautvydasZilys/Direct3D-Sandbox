#pragma once

#include "ConstantBufferField.h"

struct RenderParameters;

class ConstantBuffer
{
private:
	ComPtr<ID3D11Buffer> m_Buffer;
	vector<ConstantBufferField> m_Fields;
	int m_Size;
	
	void SetRenderParametersNoCheck();

	ConstantBuffer(const ConstantBuffer& other);

public:
	ConstantBuffer(const vector<uint8_t>& metadataBuffer, unsigned int& byteOffset);
	ConstantBuffer(ConstantBuffer&& other);

	~ConstantBuffer();

	void SetRenderParameters(const RenderParameters& renderParameters);
	ID3D11Buffer* GetPtr() const { return m_Buffer.Get(); }
};