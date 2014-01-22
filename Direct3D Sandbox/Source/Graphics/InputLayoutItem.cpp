#include "PrecompiledHeader.h"
#include "InputLayoutItem.h"
#include "Parameters.h"
#include "Tools.h"

InputLayoutItem::InputLayoutItem(ComPtr<ID3D11Device> device, const D3D11_SIGNATURE_PARAMETER_DESC& parameterDescription) :
	m_Name(parameterDescription.SemanticName),
	m_SemanticIndex(parameterDescription.SemanticIndex),
	m_ParameterOffset(VertexParameters::GetFieldByteOffset(m_Name))
{
	Assert(m_ParameterOffset != 0xFFFFFFFF);

	if (parameterDescription.Mask == 1)
	{
		switch (parameterDescription.ComponentType)
		{
		case D3D_REGISTER_COMPONENT_UINT32:
			m_Format = DXGI_FORMAT_R32_UINT;
			break;
		case D3D_REGISTER_COMPONENT_SINT32:
			m_Format = DXGI_FORMAT_R32_SINT;
			break;
		case D3D_REGISTER_COMPONENT_FLOAT32:
			m_Format = DXGI_FORMAT_R32_FLOAT;
			break;
		}

		m_Size = 4;
	}
	else if (parameterDescription.Mask <= 3)
	{
		switch (parameterDescription.ComponentType)
		{
		case D3D_REGISTER_COMPONENT_UINT32:
			m_Format = DXGI_FORMAT_R32G32_UINT;
			break;
		case D3D_REGISTER_COMPONENT_SINT32:
			m_Format = DXGI_FORMAT_R32G32_SINT;
			break;
		case D3D_REGISTER_COMPONENT_FLOAT32:
			m_Format = DXGI_FORMAT_R32G32_FLOAT;
			break;
		}

		m_Size = 8;
	}
	else if (parameterDescription.Mask <= 7)
	{
		switch (parameterDescription.ComponentType)
		{
		case D3D_REGISTER_COMPONENT_UINT32:
			m_Format = DXGI_FORMAT_R32G32B32_UINT;
			break;
		case D3D_REGISTER_COMPONENT_SINT32:
			m_Format = DXGI_FORMAT_R32G32B32_SINT;
			break;
		case D3D_REGISTER_COMPONENT_FLOAT32:
			m_Format = DXGI_FORMAT_R32G32B32_FLOAT;
			break;
		}

		m_Size = 12;
	}
	else
	{
		switch (parameterDescription.ComponentType)
		{
		case D3D_REGISTER_COMPONENT_UINT32:
			m_Format = DXGI_FORMAT_R32G32B32A32_UINT;
			break;
		case D3D_REGISTER_COMPONENT_SINT32:
			m_Format = DXGI_FORMAT_R32G32B32A32_SINT;
			break;
		case D3D_REGISTER_COMPONENT_FLOAT32:
			m_Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
			break;
		}

		m_Size = 16;
	}
}

InputLayoutItem::~InputLayoutItem()
{
}

InputLayoutItem::InputLayoutItem(InputLayoutItem&& other) :
	m_Name(std::move(other.m_Name)),
	m_SemanticIndex(other.m_SemanticIndex),
	m_Format(other.m_Format),
	m_Size(other.m_Size)
{
}

void InputLayoutItem::FillInputElementDescription(D3D11_INPUT_ELEMENT_DESC& elementDescription) const
{
	elementDescription.SemanticName = m_Name.c_str();
	elementDescription.SemanticIndex = m_SemanticIndex;
	elementDescription.Format = m_Format;
	elementDescription.InputSlot = 0;
	elementDescription.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	elementDescription.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	elementDescription.InstanceDataStepRate = 0;
}