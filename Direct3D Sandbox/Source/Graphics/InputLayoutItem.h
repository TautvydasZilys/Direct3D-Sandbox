#pragma once

class InputLayoutItem
{
private:
	string m_Name;
	unsigned int m_SemanticIndex;
	DXGI_FORMAT m_Format;
	unsigned int m_Size;
	
	InputLayoutItem(const InputLayoutItem& other);

public:
	InputLayoutItem(ComPtr<ID3D11Device> device, const D3D11_SIGNATURE_PARAMETER_DESC& parameterDescription);
	InputLayoutItem(InputLayoutItem&& other);

	~InputLayoutItem();

	const string& GetName() const { return m_Name; }
	unsigned int GetSemanticIndex() const { return m_SemanticIndex; }
	DXGI_FORMAT GetFormat() const { return m_Format; }
	unsigned int GetSize() const { return m_Size; }

	void FillInputElementDescription(D3D11_INPUT_ELEMENT_DESC& elementDescription) const;
};

