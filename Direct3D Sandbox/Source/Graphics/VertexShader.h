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
	
protected:
	virtual void Reflect(const vector<uint8_t>& shaderBuffer, const vector<uint8_t>& metadataBuffer);

public:
	VertexShader(wstring path);
	virtual ~VertexShader();
	
	ComPtr<ID3D11Buffer> CreateVertexBuffer(const ModelData& model) const;
	inline const unsigned int* GetInputLayoutSizePtr() const { return &m_InputLayoutSize; }

	virtual void SetRenderParameters(const RenderParameters& renderParameters);
};

