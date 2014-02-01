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

	auto extensionIndex = path.find_last_of('.');
	auto metadataBuffer = Tools::ReadFileToVector(path.substr(0, extensionIndex + 1) + L"shadermetadata");

	Reflect(device, shaderBuffer, metadataBuffer);
}

VertexShader::~VertexShader()
{
}

void VertexShader::Reflect(ComPtr<ID3D11Device> device, const vector<uint8_t>& shaderBuffer, const vector<uint8_t>& metadataBuffer)
{
	ShaderProgram::Reflect(device, shaderBuffer, metadataBuffer);

	ReflectInputLayout(device, shaderBuffer, metadataBuffer);
}

void VertexShader::ReflectInputLayout(ComPtr<ID3D11Device> device, const vector<uint8_t>& shaderBuffer, const vector<uint8_t>& metadataBuffer)
{
	using namespace Tools::BufferReader;

	HRESULT result;

	auto byteOffset = 4u;
	byteOffset = ReadUInt(metadataBuffer, byteOffset);
	auto numberOfInputLayoutItems = ReadUInt(metadataBuffer, byteOffset);
	
	unique_ptr<D3D11_INPUT_ELEMENT_DESC[]> inputLayoutDescription(new D3D11_INPUT_ELEMENT_DESC[numberOfInputLayoutItems]);
	m_InputLayoutItems.reserve(numberOfInputLayoutItems);
	m_InputLayoutSize = 0;

	for (auto i = 0u; i < numberOfInputLayoutItems; i++)
	{
		auto semanticName =	ReadString(metadataBuffer, byteOffset);
		auto semanticIndex = ReadUInt(metadataBuffer, byteOffset);
		auto dxgiFormat = static_cast<DXGI_FORMAT>(ReadUInt(metadataBuffer, byteOffset));
		auto itemSize = ReadUInt(metadataBuffer, byteOffset);
		auto parameterOffset = ReadUInt(metadataBuffer, byteOffset);

		m_InputLayoutItems.emplace_back(semanticName, semanticIndex, dxgiFormat, itemSize, parameterOffset);
		m_InputLayoutSize += m_InputLayoutItems[i].GetSize();
		m_InputLayoutItems[i].FillInputElementDescription(inputLayoutDescription[i]);
	}
		
	result = device->CreateInputLayout(inputLayoutDescription.get(), numberOfInputLayoutItems, 
		shaderBuffer.data(), shaderBuffer.size(), &m_InputLayout);
	Assert(result == S_OK);
}

ComPtr<ID3D11Buffer> VertexShader::CreateVertexBuffer(ComPtr<ID3D11Device> device, const ModelData& model) const
{
	HRESULT result;
	D3D11_BUFFER_DESC bufferDescription;
	D3D11_SUBRESOURCE_DATA vertexData;
	ComPtr<ID3D11Buffer> vertexBuffer;
	VertexParameters dummyParameter;

	unique_ptr<uint8_t> vertexInput(new uint8_t[m_InputLayoutSize * model.vertexCount]);
	vector<unsigned int> destinationFieldOffsets(m_InputLayoutItems.size());
	
	destinationFieldOffsets[0] = 0;
	for (auto i = 1u; i < m_InputLayoutItems.size(); i++)
	{
		destinationFieldOffsets[i] = destinationFieldOffsets[i - 1] + m_InputLayoutItems[i - 1].GetSize();
	}

	for (auto i = 0u; i < model.vertexCount; i++)
	{
		for (auto j = 0u; j < m_InputLayoutItems.size(); j++)
		{
			memcpy(vertexInput.get() + i * m_InputLayoutSize + destinationFieldOffsets[j], 
				reinterpret_cast<const uint8_t*>(&model.vertices[i]) + m_InputLayoutItems[j].GetParameterOffset(), m_InputLayoutItems[j].GetSize());
		}
	}
	
	bufferDescription.Usage = D3D11_USAGE_IMMUTABLE;
	bufferDescription.ByteWidth = static_cast<UINT>(m_InputLayoutSize * model.vertexCount);
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

	deviceContext->IASetInputLayout(m_InputLayout.Get());
	deviceContext->VSSetShader(m_Shader.Get(), nullptr, 0);
}

void VertexShader::SetConstantBuffersImpl(ComPtr<ID3D11DeviceContext> deviceContext) const
{
	deviceContext->VSSetConstantBuffers(0, static_cast<UINT>(m_ConstantBufferPtrs.size()), m_ConstantBufferPtrs.data());
}

void VertexShader::SetTexturesImpl(ComPtr<ID3D11DeviceContext> deviceContext, const vector<ID3D11ShaderResourceView*> textures) const
{
	deviceContext->VSSetShaderResources(0, static_cast<UINT>(textures.size()), textures.data());
}

void VertexShader::SetSamplersImpl(ComPtr<ID3D11DeviceContext> deviceContext) const
{
	deviceContext->VSSetSamplers(0, static_cast<UINT>(m_SamplerStates.size()), m_SamplerStates.data());
}