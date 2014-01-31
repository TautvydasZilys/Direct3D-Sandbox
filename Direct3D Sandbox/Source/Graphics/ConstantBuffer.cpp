#include "PrecompiledHeader.h"
#include "ConstantBuffer.h"
#include "Parameters.h"
#include "Tools.h"

ConstantBuffer::ConstantBuffer(ComPtr<ID3D11Device> device, const vector<uint8_t>& metadataBuffer, unsigned int& byteOffset) :
	m_Size(0)
{
	using namespace Tools::BufferReader;

	auto numberOfFields = ReadUInt(metadataBuffer, byteOffset);
	m_Fields.reserve(numberOfFields);

	for (auto i = 0u; i < numberOfFields; i++)
	{
		auto parameterOffset = ReadUInt(metadataBuffer, byteOffset);
		auto startOffset = ReadUInt(metadataBuffer, byteOffset);
		auto size = ReadUInt(metadataBuffer, byteOffset);

		m_Size += size;
		m_Fields.emplace_back(parameterOffset, startOffset, size);
	}
	
	sort(begin(m_Fields), end(m_Fields));

	HRESULT result;
	D3D11_BUFFER_DESC constantBufferDescription;

	constantBufferDescription.Usage = D3D11_USAGE_DYNAMIC;
	constantBufferDescription.ByteWidth = m_Size;
	constantBufferDescription.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constantBufferDescription.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	constantBufferDescription.MiscFlags = 0;
	constantBufferDescription.StructureByteStride = 0;

	result = device->CreateBuffer(&constantBufferDescription, nullptr, &m_Buffer);
	Assert(result == S_OK);
}

ConstantBuffer::ConstantBuffer(ConstantBuffer&& other) :
	m_Buffer(other.m_Buffer),
	m_Fields(std::move(other.m_Fields)),
	m_Size(other.m_Size)
{
	other.m_Buffer = nullptr;
}

ConstantBuffer::~ConstantBuffer()
{
}

void ConstantBuffer::SetRenderParameters(ComPtr<ID3D11DeviceContext> deviceContext, const RenderParameters& renderParameters)
{
	bool shouldCopyToGPU = false;

	for_each (begin(m_Fields), end(m_Fields), [&shouldCopyToGPU, &renderParameters](ConstantBufferField& field)
	{
		shouldCopyToGPU |= field.SetValueIfNeeded(reinterpret_cast<const uint8_t*>(&renderParameters) + field.GetParameterOffset());
	});

	if (shouldCopyToGPU)
	{
		SetRenderParametersNoCheck(deviceContext);
	}
}

void ConstantBuffer::SetRenderParametersNoCheck(ComPtr<ID3D11DeviceContext> deviceContext)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	
	result = deviceContext->Map(m_Buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	Assert(result == S_OK);
	
	for_each (begin(m_Fields), end(m_Fields), [&mappedResource](ConstantBufferField& field)
	{
		memcpy(reinterpret_cast<uint8_t*>(mappedResource.pData) + field.GetByteOffset(), field.GetValue(), field.GetSize());
	});

	deviceContext->Unmap(m_Buffer.Get(), 0);
}