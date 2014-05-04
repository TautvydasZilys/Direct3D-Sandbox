#include "PrecompiledHeader.h"
#include "InputLayoutItem.h"
#include "Parameters.h"
#include "Tools.h"

InputLayoutItem::InputLayoutItem(const string& semanticName, unsigned int semanticIndex, DXGI_FORMAT dxgiFormat, 
									unsigned int itemSize, unsigned int parameterOffset) :
	m_Name(semanticName),
	m_SemanticIndex(semanticIndex),
	m_Format(dxgiFormat),
	m_Size(itemSize),
	m_ParameterOffset(parameterOffset)
{
	Assert(m_ParameterOffset != 0xFFFFFFFF);
}

InputLayoutItem::~InputLayoutItem()
{
}

InputLayoutItem::InputLayoutItem(InputLayoutItem&& other) :
	m_Name(std::move(other.m_Name)),
	m_SemanticIndex(other.m_SemanticIndex),
	m_Format(other.m_Format),
	m_Size(other.m_Size),
	m_ParameterOffset(other.m_ParameterOffset)
{
}

void InputLayoutItem::FillInputElementDescription(D3D11_INPUT_ELEMENT_DESC& elementDescription) const
{
	elementDescription.SemanticName = m_Name.c_str();
	elementDescription.SemanticIndex = m_SemanticIndex;
	elementDescription.Format = m_Format;
	elementDescription.InputSlot = m_SemanticIndex;
	elementDescription.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	elementDescription.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	elementDescription.InstanceDataStepRate = 0;
}