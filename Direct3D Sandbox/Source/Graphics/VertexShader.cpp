#include "PrecompiledHeader.h"
#include "Direct3D.h"
#include "Parameters.h"
#include "Tools.h"
#include "VertexShader.h"

VertexShader::VertexShader(wstring path)
{
	HRESULT result;

	auto shaderBuffer = Tools::ReadFileToVector(path);
	result = GetD3D11Device()->CreateVertexShader(&shaderBuffer[0], shaderBuffer.size(), nullptr, &m_Shader);
	Assert(result == S_OK);

	auto extensionIndex = path.find_last_of('.');
	auto metadataBuffer = Tools::ReadFileToVector(path.substr(0, extensionIndex + 1) + L"shadermetadata");

	Reflect(shaderBuffer, metadataBuffer);
}

VertexShader::~VertexShader()
{
}

void VertexShader::Reflect(const vector<uint8_t>& shaderBuffer, const vector<uint8_t>& metadataBuffer)
{
	ShaderProgram::Reflect(shaderBuffer, metadataBuffer);

	ReflectInputLayout(shaderBuffer, metadataBuffer);
}

void VertexShader::ReflectInputLayout(const vector<uint8_t>& shaderBuffer, const vector<uint8_t>& metadataBuffer)
{
	using namespace Tools::BufferReader;

	HRESULT result;

	auto byteOffset = 4u;
	byteOffset = ReadUInt(metadataBuffer, byteOffset);
	auto numberOfInputLayoutItems = ReadUInt(metadataBuffer, byteOffset);
	
	unique_ptr<D3D11_INPUT_ELEMENT_DESC[]> inputLayoutDescription(new D3D11_INPUT_ELEMENT_DESC[numberOfInputLayoutItems]);
	unsigned int lastSemanticIndex = 0;

	// Create input layout items
	for (auto i = 0u; i < numberOfInputLayoutItems; i++)
	{
		auto semanticName =	ReadString(metadataBuffer, byteOffset);
		auto semanticIndex = ReadUInt(metadataBuffer, byteOffset);
		auto dxgiFormat = static_cast<DXGI_FORMAT>(ReadUInt(metadataBuffer, byteOffset));
		auto itemSize = ReadUInt(metadataBuffer, byteOffset);
		auto parameterOffset = ReadUInt(metadataBuffer, byteOffset);
		
		if (lastSemanticIndex > semanticIndex)
		{
			wstringstream errorMsg;
			errorMsg << "Error: semantic index of vertex input element at slot " << i << " is higher than the last one: " << lastSemanticIndex << L" > " 
				<< semanticIndex << "." << endl;

			Tools::FatalError(errorMsg.str());
		}
		
		while (semanticIndex >= m_InputLayoutItems.size())
		{
			m_InputLayoutStrides.push_back(0);
			m_InputLayoutItems.emplace_back();
		}

		m_InputLayoutItems[semanticIndex].emplace_back(semanticName, semanticIndex, dxgiFormat, itemSize, parameterOffset);
		m_InputLayoutItems[semanticIndex].back().FillInputElementDescription(inputLayoutDescription[i]);
		
		m_InputLayoutStrides[semanticIndex] += itemSize;
		lastSemanticIndex = semanticIndex;
	}
	
	// Fill description
	auto descriptionIndex = 0;
	for (auto semanticIndex = 0u; semanticIndex < m_InputLayoutItems.size(); semanticIndex++)
	{
		auto nItems = m_InputLayoutItems[semanticIndex].size();

		for (auto i = 0u; i < nItems; i++)
		{
			m_InputLayoutItems[semanticIndex][i].FillInputElementDescription(inputLayoutDescription[descriptionIndex++]);
		}
	}

	result = GetD3D11Device()->CreateInputLayout(inputLayoutDescription.get(), numberOfInputLayoutItems, 
		shaderBuffer.data(), shaderBuffer.size(), &m_InputLayout);
	Assert(result == S_OK);
}

unique_ptr<uint8_t[]> VertexShader::ArrangeVertexBufferData(unsigned int vertexCount, const VertexParameters vertices[], unsigned int semanticIndex) const
{	
	const auto& inputLayoutItems = m_InputLayoutItems[semanticIndex];
	const auto layoutSize = m_InputLayoutStrides[semanticIndex];
	
	unique_ptr<uint8_t[]> vertexInput(new uint8_t[layoutSize * vertexCount]);
	vector<unsigned int> destinationFieldOffsets(inputLayoutItems.size());

	destinationFieldOffsets[0] = 0;
	for (auto i = 1u; i < inputLayoutItems.size(); i++)
	{
		destinationFieldOffsets[i] = destinationFieldOffsets[i - 1] + inputLayoutItems[i - 1].GetSize();
	}

	for (auto i = 0u; i < vertexCount; i++)
	{
		for (auto j = 0u; j < inputLayoutItems.size(); j++)
		{
			const auto& item = inputLayoutItems[j];

			memcpy(vertexInput.get() + i * layoutSize + destinationFieldOffsets[j], 
				reinterpret_cast<const uint8_t*>(&vertices[i]) + item.GetParameterOffset(), item.GetSize());
		}
	}
	
	return vertexInput;
}

ComPtr<ID3D11Buffer> VertexShader::CreateVertexBuffer(unsigned int vertexCount, D3D11_USAGE usage, const D3D11_SUBRESOURCE_DATA* vertexData,
	unsigned int semanticIndex) const
{
	HRESULT result;
	D3D11_BUFFER_DESC bufferDescription;
	ComPtr<ID3D11Buffer> vertexBuffer;

	bufferDescription.Usage = usage;
	bufferDescription.ByteWidth = static_cast<UINT>(m_InputLayoutStrides[semanticIndex] * vertexCount);
	bufferDescription.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDescription.CPUAccessFlags = (usage == D3D11_USAGE::D3D11_USAGE_DYNAMIC) ? D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE : 0;
	bufferDescription.MiscFlags = 0;
	bufferDescription.StructureByteStride = 0;

	result = GetD3D11Device()->CreateBuffer(&bufferDescription, vertexData, &vertexBuffer);
	Assert(result == S_OK);

	return vertexBuffer;
}

ComPtr<ID3D11Buffer> VertexShader::CreateVertexBufferAndUploadData(unsigned int vertexCount, 
	const VertexParameters vertices[], D3D11_USAGE usage, unsigned int semanticIndex) const
{
	if (semanticIndex >= m_InputLayoutStrides.size()) return nullptr;

	D3D11_SUBRESOURCE_DATA vertexData;
	auto verterBufferData = ArrangeVertexBufferData(vertexCount, vertices, semanticIndex);

	vertexData.pSysMem = verterBufferData.get();
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	return CreateVertexBuffer(vertexCount, usage, &vertexData, semanticIndex);
}

void VertexShader::UploadVertexData(ID3D11Buffer* vertexBuffer, unsigned int vertexCount, const VertexParameters vertices[], unsigned int semanticIndex) const
{
	if (semanticIndex >= m_InputLayoutStrides.size()) return;

	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	auto deviceContext = GetD3D11DeviceContext();
	auto vertexBufferData = ArrangeVertexBufferData(vertexCount, vertices, semanticIndex);	

	result = deviceContext->Map(vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	Assert(result == S_OK);

	memcpy(mappedResource.pData, vertexBufferData.get(), m_InputLayoutStrides[semanticIndex] * vertexCount);
	deviceContext->Unmap(vertexBuffer, 0);
}

ComPtr<ID3D11Buffer> VertexShader::CreateVertexBuffer(unsigned int vertexCount, D3D11_USAGE usage, unsigned int semanticIndex) const
{
	if (semanticIndex >= m_InputLayoutStrides.size()) return nullptr;

	Assert(usage != D3D11_USAGE::D3D11_USAGE_IMMUTABLE);

	return CreateVertexBuffer(vertexCount, usage, nullptr, semanticIndex);
}

void VertexShader::SetRenderParameters(const RenderParameters& renderParameters)
{
	ShaderProgram::SetRenderParameters(renderParameters);
	
	static const VertexShader* shaderWhichLastSet = nullptr;

	if (shaderWhichLastSet != this)
	{
		shaderWhichLastSet = this;

		auto deviceContext = GetD3D11DeviceContext();
		deviceContext->IASetInputLayout(m_InputLayout.Get());
		deviceContext->VSSetShader(m_Shader.Get(), nullptr, 0);
	}
}

void VertexShader::SetConstantBuffersImpl() const
{
	static const VertexShader* shaderWhichLastSet = nullptr;

	if (shaderWhichLastSet != this)
	{
		shaderWhichLastSet = this;
		GetD3D11DeviceContext()->VSSetConstantBuffers(0, static_cast<UINT>(m_ConstantBufferPtrs.size()), m_ConstantBufferPtrs.data());
	}
}

void VertexShader::SetTexturesImpl()
{
	GetD3D11DeviceContext()->VSSetShaderResources(0, static_cast<UINT>(m_Textures.size()), m_Textures.data());
}

void VertexShader::SetSamplersImpl() const
{
	static const VertexShader* shaderWhichLastSet = nullptr;

	if (shaderWhichLastSet != this)
	{
		shaderWhichLastSet = this;
		GetD3D11DeviceContext()->VSSetSamplers(0, static_cast<UINT>(m_SamplerStates.size()), m_SamplerStates.data());
	}
}