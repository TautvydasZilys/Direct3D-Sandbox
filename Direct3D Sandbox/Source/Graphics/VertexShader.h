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
	
	void ReflectInputLayout(ComPtr<ID3D11Device> device, const vector<uint8_t>& shaderBuffer, const vector<uint8_t>& metadataBuffer);
	
	virtual void SetConstantBuffersImpl(ComPtr<ID3D11DeviceContext> deviceContext) const;
	virtual void SetTexturesImpl(ComPtr<ID3D11DeviceContext> deviceContext);
	virtual void SetSamplersImpl(ComPtr<ID3D11DeviceContext> deviceContext) const;
	
protected:
	virtual void Reflect(ComPtr<ID3D11Device> device, const vector<uint8_t>& shaderBuffer, const vector<uint8_t>& metadataBuffer);

public:
	VertexShader(ComPtr<ID3D11Device> device, wstring path);
	virtual ~VertexShader();
	
	ComPtr<ID3D11Buffer> CreateVertexBuffer(ComPtr<ID3D11Device> device, const ModelData& model) const;
	inline const unsigned int* GetInputLayoutSizePtr() const { return &m_InputLayoutSize; }

	virtual void SetRenderParameters(ComPtr<ID3D11DeviceContext> deviceContext, const RenderParameters& renderParameters);
};

