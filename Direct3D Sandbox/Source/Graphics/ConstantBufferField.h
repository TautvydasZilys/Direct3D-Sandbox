#pragma once

class ConstantBufferField
{
private:
	string m_Name;
	unsigned int m_ByteOffset;
	unsigned int m_Size;
	unique_ptr<uint8_t[]> m_Value;

	ConstantBufferField(const ConstantBufferField& other);
	ConstantBufferField& operator=(const ConstantBufferField& other);

public:
	ConstantBufferField(string name, unsigned int byteOffset, unsigned int size) : 
		m_Name(std::move(name)), m_ByteOffset(byteOffset), m_Size(size), m_Value(new uint8_t[size]) {}

	ConstantBufferField(ConstantBufferField&& other) : 
		m_Name(std::move(other.m_Name)), m_ByteOffset(other.m_ByteOffset), m_Size(other.m_Size), m_Value(std::move(other.m_Value)) {}

	ConstantBufferField& operator=(ConstantBufferField&& other);

	inline bool operator<(const ConstantBufferField& other) const { return m_ByteOffset < other.m_ByteOffset; }

	inline const string& GetName() const { return m_Name; }
	inline unsigned int GetByteOffset() const { return m_ByteOffset; }
	inline unsigned int GetSize() const { return m_Size; }
	inline const void* GetValue() const { return m_Value.get(); }

	bool SetValueIfNeeded(const void* value);
};
