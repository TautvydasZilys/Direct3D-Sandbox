#include "PrecompiledHeader.h"
#include "ConstantBufferField.h"
#include "RenderParameters.h"
#include "Tools.h"

ConstantBufferField::ConstantBufferField(const string& name, unsigned int byteOffset, unsigned int size) : 
#if DEBUG
	m_Name(std::move(name)), 
#endif
	m_ByteOffset(byteOffset), 
	m_Size(size), 
	m_Value(new uint8_t[size])
{
	m_ParameterOffset = RenderParameters::GetFieldByteOffset(name);
	Assert(m_ParameterOffset != 0xFFFFFFFF);
}

ConstantBufferField::ConstantBufferField(ConstantBufferField&& other) :
#if DEBUG
	m_Name(std::move(other.m_Name)),
#endif
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

#if DEBUG
	m_Name = std::move(other.m_Name);
#endif

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