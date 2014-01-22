#include "PrecompiledHeader.h"
#include "Parameters.h"
#include "Tools.h"
#include "VertexShader.h"

VertexShader::VertexShader(ComPtr<ID3D11Device> device, wstring path)
{
	HRESULT result;

	auto shaderBuffer = Tools::ReadFileToVector(path);
	result = device->CreateVertexShader(&shaderBuffer[0], shaderBuffer.size(), nullptr, &m_Shader);
	Assert(result == S_OK);

	Reflect(device, shaderBuffer);
}


VertexShader::~VertexShader()
{
}

void VertexShader::ReflectVirtual(ComPtr<ID3D11Device> device, const vector<uint8_t>& shaderBuffer, ComPtr<ID3D11ShaderReflection> shaderReflection, 
									const D3D11_SHADER_DESC& shaderDescription)
{
	ShaderProgram::ReflectVirtual(device, shaderBuffer, shaderReflection, shaderDescription);

	ReflectInputLayout(device, shaderBuffer, shaderReflection, shaderDescription);
}

void VertexShader::ReflectInputLayout(ComPtr<ID3D11Device> device, const vector<uint8_t>& shaderBuffer, ComPtr<ID3D11ShaderReflection> shaderReflection, 
										const D3D11_SHADER_DESC& shaderDescription)
{
	HRESULT result;
	D3D11_SIGNATURE_PARAMETER_DESC parameterDescription;
	unique_ptr<D3D11_INPUT_ELEMENT_DESC[]> inputLayoutDescription(new D3D11_INPUT_ELEMENT_DESC[shaderDescription.InputParameters]);
	m_InputLayoutSize = 0;

	for (auto i = 0u; i < shaderDescription.InputParameters; i++)
	{
		result = shaderReflection->GetInputParameterDesc(i, &parameterDescription);
		Assert(result == S_OK);

		m_InputLayoutItems.emplace_back(device, parameterDescription);
		m_InputLayoutSize += m_InputLayoutItems[i].GetSize();
		m_InputLayoutItems[i].FillInputElementDescription(inputLayoutDescription[i]);
	}
		
	result = device->CreateInputLayout(inputLayoutDescription.get(), shaderDescription.InputParameters, 
		shaderBuffer.data(), shaderBuffer.size(), &m_InputLayout);
	Assert(result);
}

ComPtr<ID3D11Buffer> VertexShader::CreateVertexBuffer(ComPtr<ID3D11Device> device, const vector<VertexParameters>& vertices) const
{
	HRESULT result;
	D3D11_BUFFER_DESC bufferDescription;
	D3D11_SUBRESOURCE_DATA vertexData;
	ComPtr<ID3D11Buffer> vertexBuffer;
	VertexParameters dummyParameter;

	unique_ptr<uint8_t> vertexInput(new uint8_t[m_InputLayoutSize * vertices.size()]);
	vector<unsigned int> destinationFieldOffsets(m_InputLayoutItems.size());
	
	destinationFieldOffsets[0] = 0;
	for (auto i = 1u; i < m_InputLayoutItems.size(); i++)
	{
		destinationFieldOffsets[i] = destinationFieldOffsets[i - 1] + m_InputLayoutItems[i - 1].GetSize();
	}

	for (auto i = 0u; i < vertices.size(); i++)
	{
		for (auto j = 0u; j < m_InputLayoutItems.size(); j++)
		{
			memcpy(vertexInput.get() + i * m_InputLayoutSize + destinationFieldOffsets[j], 
				reinterpret_cast<const uint8_t*>(&vertices[i]) + m_InputLayoutItems[i].GetParameterOffset(), m_InputLayoutItems[i].GetSize());
		}
	}
	
	bufferDescription.Usage = D3D11_USAGE_IMMUTABLE;
	bufferDescription.ByteWidth = static_cast<UINT>(m_InputLayoutSize * vertices.size());
	bufferDescription.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDescription.CPUAccessFlags = 0;
	bufferDescription.MiscFlags = 0;
	bufferDescription.StructureByteStride = 0;

	vertexData.pSysMem = vertexInput.get();
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&bufferDescription, &vertexData, &vertexBuffer);
	Assert(result == S_OK);

	return vertexBuffer;
}

void VertexShader::SetRenderParameters(ComPtr<ID3D11DeviceContext> deviceContext, const RenderParameters& renderParameters)
{
	ShaderProgram::SetRenderParameters(deviceContext, renderParameters);

	deviceContext->VSSetShader(m_Shader.Get(), nullptr, 0);
}