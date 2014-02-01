#include "PrecompiledHeader.h"
#include "ConstantBufferField.h"
#include "Parameters.h"
#include "Tools.h"

ConstantBufferField::ConstantBufferField(unsigned int parameterOffset, unsigned int byteOffset, unsigned int size) : 
	m_ParameterOffset(parameterOffset),
	m_ByteOffset(byteOffset), 
	m_Size(size), 
	m_Value(new uint8_t[size])
{
	Assert(m_ParameterOffset != 0xFFFFFFFF);
}

ConstantBufferField::ConstantBufferField(ConstantBufferField&& other) :
	m_ParameterOffset(other.m_ParameterOffset),
	m_ByteOffset(other.m_ByteOffset),
	m_Size(other.m_Size),
	m_Value(std::move(other.m_Value))
{
}

ConstantBufferField& ConstantBufferField::operator=(ConstantBufferField&& other)
{
	m_ParameterOffset = other.m_ParameterOffset;
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