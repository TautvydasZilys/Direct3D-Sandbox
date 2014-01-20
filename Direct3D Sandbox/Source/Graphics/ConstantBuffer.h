#pragma once

#include "RenderParameters.h"

class ConstantBufferField
{
private:
	string m_Name;
	unsigned int m_ByteOffset;
	unsigned int m_Size;
	unique_ptr<uint8_t[]> m_Value;

	ConstantBufferField(const ConstantBufferField& other);

public:
	ConstantBufferField(string name, unsigned int byteOffset, unsigned int size) : 
		m_Name(std::move(name)), m_ByteOffset(byteOffset), m_Size(size), m_Value(new uint8_t[size]) {}

	ConstantBufferField(ConstantBufferField&& other) : 
		m_Name(std::move(other.m_Name)), m_ByteOffset(other.m_ByteOffset), m_Size(other.m_Size), m_Value(std::move(other.m_Value)) {}

	inline const string& GetName() const { return m_Name; }
	inline unsigned int GetByteOffset() const { return m_ByteOffset; }
	inline unsigned int GetSize() const { return m_Size; }
	inline const void* GetValue() const { return m_Value.get(); }

	bool SetValueIfNeeded(const void* value);
};

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

	void SetRenderParameters(ComPtr<ID3D11DeviceContext> deviceContext, const GlobalParameters& globalParameters, const ModelParameters& modelParameters);
};