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
	
	virtual void ReflectVirtual(ComPtr<ID3D11Device> device, const vector<uint8_t>& shaderBuffer, ComPtr<ID3D11ShaderReflection> shaderReflection, 
		const D3D11_SHADER_DESC& shaderDescription);

	void ReflectInputLayout(ComPtr<ID3D11Device> device, const vector<uint8_t>& shaderBuffer, ComPtr<ID3D11ShaderReflection> shaderReflection, 
		const D3D11_SHADER_DESC& shaderDescription);

public:
	VertexShader(ComPtr<ID3D11Device> device, wstring path);
	virtual ~VertexShader();

	ComPtr<ID3D11Buffer> CreateVertexBuffer(ComPtr<ID3D11Device> device, const vector<VertexParameters>& vertices) const;
	
	virtual void SetRenderParameters(ComPtr<ID3D11DeviceContext> deviceContext, const RenderParameters& renderParameters);
};

