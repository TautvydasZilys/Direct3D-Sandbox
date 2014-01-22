#pragma once

class InputLayoutItem
{
private:
	string m_Name;
	unsigned int m_SemanticIndex;
	DXGI_FORMAT m_Format;
	unsigned int m_Size;
	unsigned int m_ParameterOffset;
	
	InputLayoutItem(const InputLayoutItem& other);

public:
	InputLayoutItem(ComPtr<ID3D11Device> device, const D3D11_SIGNATURE_PARAMETER_DESC& parameterDescription);
	InputLayoutItem(InputLayoutItem&& other);

	~InputLayoutItem();

	unsigned int GetSemanticIndex() const { return m_SemanticIndex; }
	DXGI_FORMAT GetFormat() const { return m_Format; }
	unsigned int GetSize() const { return m_Size; }
	unsigned int GetParameterOffset() const { return m_ParameterOffset; }

	void FillInputElementDescription(D3D11_INPUT_ELEMENT_DESC& elementDescription) const;
};
