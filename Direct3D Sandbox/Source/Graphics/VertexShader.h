#pragma once

#include "InputLayoutItem.h"
#include "ShaderProgram.h"

struct VertexParameters;
struct RenderParameters;

class VertexShader :
	public ShaderProgram
{
private:
	ComPtr<ID3D11VertexShader> m_Shader;
	ComPtr<ID3D11InputLayout> m_InputLayout;
	vector<InputLayoutItem> m_InputLayoutItems;
	unsigned int m_InputLayoutSize;
	
	void ReflectInputLayout(const vector<uint8_t>& shaderBuffer, const vector<uint8_t>& metadataBuffer);
	
	virtual void SetConstantBuffersImpl() const;
	virtual void SetTexturesImpl();
	virtual void SetSamplersImpl() const;

	unique_ptr<uint8_t[]> ArrangeVertexBufferData(unsigned int vertexCount, const VertexParameters vertices[]) const;
	ComPtr<ID3D11Buffer> CreateVertexBuffer(unsigned int vertexCount, D3D11_USAGE usage, const D3D11_SUBRESOURCE_DATA* vertexData) const;
	
protected:
	virtual void Reflect(const vector<uint8_t>& shaderBuffer, const vector<uint8_t>& metadataBuffer);

public:
	VertexShader(wstring path);
	virtual ~VertexShader();
	
	ComPtr<ID3D11Buffer> CreateVertexBufferAndUploadData(unsigned int vertexCount, const VertexParameters vertices[], D3D11_USAGE usage) const;
	ComPtr<ID3D11Buffer> CreateVertexBuffer(unsigned int vertexCount, D3D11_USAGE usage) const;
	void UploadVertexData(ID3D11Buffer* vertexBuffer, unsigned int vertexCount, const VertexParameters vertices[]) const;

	inline const unsigned int* GetInputLayoutSizePtr() const { return &m_InputLayoutSize; }

	virtual void SetRenderParameters(const RenderParameters& renderParameters);
};

