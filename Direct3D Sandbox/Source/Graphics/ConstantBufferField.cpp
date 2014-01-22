#include "PrecompiledHeader.h"
#include "ConstantBufferField.h"

ConstantBufferField& ConstantBufferField::operator=(ConstantBufferField&& other)
{
	m_Name = std::move(other.m_Name);
	m_ByteOffset = other.m_ByteOffset;
	m_Size = other.m_Size;
	m_Value = std::move(other.m_Value);

	return *this;
}

bool ConstantBufferField::SetValueIfNeeded(const void* value)
{
	auto shouldSet = memcmp(m_Value.get(), value, m_Size) != 0;

	if (shouldSet)
	{
		memcpy(m_Value.get(), value, m_Size);
	}

	return shouldSet;
}