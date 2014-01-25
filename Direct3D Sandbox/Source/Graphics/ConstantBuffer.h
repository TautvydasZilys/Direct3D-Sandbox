#pragma once

#include "ConstantBufferField.h"

struct RenderParameters;

class ConstantBuffer
{
private:
	ComPtr<ID3D11Buffer> m_Buffer;
	vector<ConstantBufferField> m_Fields;
	int m_Size;

#if DEBUG
	string m_Name;
#endif

	void SetRenderParametersNoCheck(ComPtr<ID3D11DeviceContext> deviceContext);

	ConstantBuffer(const ConstantBuffer& other);
public:
	ConstantBuffer(ComPtr<ID3D11Device> device, ID3D11ShaderReflectionConstantBuffer* bufferReflection);
	ConstantBuffer(ConstantBuffer&& other);

	~ConstantBuffer();

	void SetRenderParameters(ComPtr<ID3D11DeviceContext> deviceContext, const RenderParameters& renderParameters);
	ID3D11Buffer* GetPtr() const { return m_Buffer.Get(); }
};