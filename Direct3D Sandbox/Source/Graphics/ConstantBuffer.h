#pragma once

class ConstantBufferField
{
public:
	ConstantBufferField(int byteOffset, string name) : byteOffset(byteOffset), name(name) {}

	int byteOffset;
	string name;
};

class ConstantBuffer
{
private:
	vector<ConstantBufferField> m_Fields;

public:
	ConstantBuffer(ComPtr<ID3D11ShaderReflectionConstantBuffer> bufferReflection);
	~ConstantBuffer();
};

