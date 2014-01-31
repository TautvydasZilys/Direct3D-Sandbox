#pragma once

class ConstantBufferField
{
private:
	unsigned int m_ParameterOffset;
	unsigned int m_ByteOffset;
	unsigned int m_Size;
	unique_ptr<uint8_t[]> m_Value;
	
	ConstantBufferField(const ConstantBufferField& other);
	ConstantBufferField& operator=(const ConstantBufferField& other);

public:
	ConstantBufferField(unsigned int parameterOffset, unsigned int byteOffset, unsigned int size);
	ConstantBufferField(ConstantBufferField&& other);
	ConstantBufferField& operator=(ConstantBufferField&& other);

	inline bool operator<(const ConstantBufferField& other) const { return m_ByteOffset < other.m_ByteOffset; }

	inline unsigned int GetParameterOffset() const { return m_ParameterOffset; }
	inline unsigned int GetByteOffset() const { return m_ByteOffset; }
	inline unsigned int GetSize() const { return m_Size; }
	inline const void* GetValue() const { return m_Value.get(); }

	bool SetValueIfNeeded(const void* value);
};
